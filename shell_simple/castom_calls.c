#include "shell.h"

char *getArguments(char *commandLine, int *exitStatus);
int executeArguments(char **arguments, char **front, int *exitStatus);
int processArguments(char **arguments, char **front, int *exitStatus);
int handleArguments(int *exitStatus);
int validateArguments(char **arguments);

/**
 * getArguments - Gets a command from standard input.
 * @commandLine: A buffer to store the command.
 * @exitStatus: The return value of the last executed command.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *getArguments(char *commandLine, int *exitStatus)
{
	size_t bufferSize = 0;
	ssize_t bytesRead;
	char *prompt = "$ ";

	if (commandLine)
		free(commandLine);

	bytesRead = custom_getline(&commandLine, &bufferSize, STDIN_FILENO);
	if (bytesRead == -1)
		return (NULL);
	if (bytesRead == 1)
	{
		history++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (getArguments(commandLine, exitStatus));
	}

	commandLine[bytesRead - 1] = '\0';
	replaceVariables(&commandLine, exitStatus);
	processLine(&commandLine, bytesRead);

	return (commandLine);
}

/**
 * executeArguments - Partitions operators from commands and calls them.
 * @arguments: An array of arguments.
 * @front: A double pointer to the beginning of arguments.
 * @exitStatus: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int executeArguments(char **arguments, char **front, int *exitStatus)
{
	int retValue, index;

	if (!arguments[0])
		return (*exitStatus);
	for (index = 0; arguments[index]; index++)
	{
		if (custom_strncmp(arguments[index], "||", 2) == 0)
		{
			free(arguments[index]);
			arguments[index] = NULL;
			arguments = replaceAliases(arguments);
			retValue = processArguments(arguments, front, exitStatus);
			if (*exitStatus != 0)
			{
				arguments = &arguments[++index];
				index = 0;
			}
			else
			{
				for (index++; arguments[index]; index++)
					free(arguments[index]);
				return (retValue);
			}
		}
		else if (custom_strncmp(arguments[index], "&&", 2) == 0)
		{
			free(arguments[index]);
			arguments[index] = NULL;
			arguments = replaceAliases(arguments);
			retValue = processArguments(arguments, front, exitStatus);
			if (*exitStatus == 0)
			{
				arguments = &arguments[++index];
				index = 0;
			}
			else
			{
				for (index++; arguments[index]; index++)
					free(arguments[index]);
				return (retValue);
			}
		}
	}
	arguments = replaceAliases(arguments);
	retValue = processArguments(arguments, front, exitStatus);
	return (retValue);
}

/**
 * processArguments - Calls the execution of a command.
 * @arguments: An array of arguments.
 * @front: A double pointer to the beginning of arguments.
 * @exitStatus: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int processArguments(char **arguments, char **front, int *exitStatus)
{
	int retValue, i;
	int (*builtin)(char **arguments, char **front);

	builtin = getBuiltin(arguments[0]);

	if (builtin)
	{
		retValue = builtin(arguments + 1, front);
		if (retValue != EXIT)
			*exitStatus = retValue;
	}
	else
	{
		*exitStatus = custom_execute(arguments, front);
		retValue = *exitStatus;
	}

	history++;

	for (i = 0; arguments[i]; i++)
		free(arguments[i]);

	return (retValue);
}

/**
 * handleArguments - Gets, calls, and runs the execution of a command.
 * @exitStatus: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         Otherwise - The exit value of the last executed command.
 */
int handleArguments(int *exitStatus)
{
	int retValue = 0, index;
	char **arguments, *commandLine = NULL, **front;

	commandLine = getArguments(commandLine, exitStatus);
	if (!commandLine)
		return (END_OF_FILE);

	arguments = custom_strtok(commandLine, " ");
	free(commandLine);
	if (!arguments)
		return (retValue);
	if (validateArguments(arguments) != 0)
	{
		*exitStatus = 2;
		custom_free_args(arguments, arguments);
		return (*exitStatus);
	}
	front = arguments;

	for (index = 0; arguments[index]; index++)
	{
		if (custom_strncmp(arguments[index], ";", 1) == 0)
		{
			free(arguments[index]);
			arguments[index] = NULL;
			retValue = executeArguments(arguments, front, exitStatus);
			arguments = &arguments[++index];
			index = 0;
		}
	}
	if (arguments)
		retValue = executeArguments(arguments, front, exitStatus);

	free(front);
	return (retValue);
}

/**
 * validateArguments - Checks if there are any leading ';', ';;', '&&', or '||'.
 * @arguments: 2D pointer to tokenized commands and arguments.
 *
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 *         Otherwise - 0.
 */
int validateArguments(char **arguments)
{
	size_t i;
	char *current, *next;

	for (i = 0; arguments[i]; i++)
	{
		current = arguments[i];
		if (current[0] == ';' || current[0] == '&' || current[0] == '|')
		{
			if (i == 0 || current[1] == ';')
				return (custom_create_error(&arguments[i], 2));
			next = arguments[i + 1];
			if (next && (next[0] == ';' || next[0] == '&' || next[0] == '|'))
				return (custom_create_error(&arguments[i + 1], 2));
		}
	}
	return (0);
}

