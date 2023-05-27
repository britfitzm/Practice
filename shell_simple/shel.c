#include "shell.h"

void handleSignal(int sig);
int executeCommand(char **arguments, char **front);

/**
 * handleSignal - Prints a new prompt upon a signal.
 * @sig: The signal.
 */
void handleSignal(int sig)
{
	char *newPrompt = "\n$ ";

	(void)sig;
	signal(SIGINT, handleSignal);
	write(STDIN_FILENO, newPrompt, 3);
}

/**
 * executeCommand - Executes a command in a child process.
 * @arguments: An array of arguments.
 * @front: A double pointer to the beginning of arguments.
 *
 * Return: If an error occurs - a corresponding error code.
 *         O/w - The exit value of the last executed command.
 */
int executeCommand(char **arguments, char **front)
{
	pid_t childPid;
	int status, flag = 0, ret = 0;
	char *command = arguments[0];

	if (command[0] != '/' && command[0] != '.')
	{
		flag = 1;
		command = getFullPathDir(command);
	}

	if (!command || (access(command, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (createError(arguments, 126));
		else
			ret = (createError(arguments, 127));
	}
	else
	{
		childPid = fork();
		if (childPid == -1)
		{
			if (flag)
				free(command);
			perror("Error child:");
			return (1);
		}
		if (childPid == 0)
		{
			execve(command, arguments, environment);
			if (errno == EACCES)
				ret = (createError(arguments, 126));
			freeEnvironment();
			freeArguments(arguments, front);
			freeAliasList(aliases);
			_exit(ret);
		}
		else
		{
			wait(&status);
			ret = WEXITSTATUS(status);
		}
	}
	if (flag)
		free(command);
	return (ret);
}

/**
 * main - Runs a simple UNIX command interpreter.
 * @argc: The number of arguments supplied to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: The return value of the last executed command.
 */
int main(int argc, char *argv[])
{
	int ret = 0, retn;
	int *executionRet = &retn;
	char *prompt = "$ ", *newLine = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, handleSignal);

	*executionRet = 0;
	environment = copyEnvironment();
	if (!environment)
		exit(-100);

	if (argc != 1)
	{
		ret = processFileCommands(argv[1], executionRet);
		freeEnvironment();
		freeAliasList(aliases);
		return (*executionRet);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = handleArguments(executionRet);
		freeEnvironment();
		freeAliasList(aliases);
		return (*executionRet);
	}

	while (1)
	{
		write(STDOUT_FILENO, prompt, 2);
		ret = handleArguments(executionRet);
		if (ret == END_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, newLine, 1);
			freeEnvironment();
			freeAliasList(aliases);
			exit(*executionRet);
		}
	}

	freeEnvironment();
	freeAliasList(aliases);
	return (*executionRet);
}

