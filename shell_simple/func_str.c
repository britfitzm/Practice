#include "shell.h"

/**
 * get_builtin - Matches a command with a corresponding
 *              shellby builtin function.
 * @command: The command to match.
 *
 * Return: A function pointer to the corresponding builtin.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{"exitShellby", shellby_exit},
		{"envShellby", shellby_env},
		{"setenvShellby", shellby_setenv},
		{"unsetenvShellby", shellby_unsetenv},
		{"cdShellby", shellby_cd},
		{"aliasShellby", shellby_alias},
		{"helpShellby", shellby_help},
		{NULL, NULL}
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (_strcmp(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * shellby_exit - Causes normal process termination
 *                for the shellby shell.
 * @args: An array of arguments containing the exit value.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If there are no arguments - -3.
 *         If the given exit value is invalid - 2.
 *         O/w - exits with the given status value.
 *
 * Description: Upon returning -3, the program exits back in the main function.
 */
int shellby_exit(char **args, char **front)
{
	int i, lenOfInt = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			lenOfInt++;
		}
		for (; args[0][i]; i++)
		{
			if (i <= lenOfInt && args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (createError(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (createError(--args, 2));
	args -= 1;
	freeArgs(args, front);
	freeEnv();
	freeAliasList(aliases);
	exit(num);
}

/**
 * shellby_cd - Changes the current directory of the shellby process.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If the given string is not a directory - 2.
 *         If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_cd(char **args, char __attribute__((__unused__)) **front)
{
	char **dirInfo, *newLine = "\n";
	char *oldPwd = NULL, *pwd = NULL;
	struct stat dir;

	oldPwd = getcwd(oldPwd, 0);
	if (!oldPwd)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
				(chdir(*_getenv("OLDPWD") + 7));
			else
			{
				free(oldPwd);
				return (createError(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(oldPwd);
				return (createError(args, 2));
			}
		}
	}
	else
	{
		if (_getenv("HOME") != NULL)
			chdir(*(_getenv("HOME")) + 5);
	}

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	dirInfo = malloc(sizeof(char *) * 2);
	if (!dirInfo)
		return (-1);

	dirInfo[0] = "OLDPWD";
	dirInfo[1] = oldPwd;
	if (shellby_setenv(dirInfo, dirInfo) == -1)
		return (-1);

	dirInfo[0] = "PWD";
	dirInfo[1] = pwd;
	if (shellby_setenv(dirInfo, dirInfo) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, _strlen(pwd));
		write(STDOUT_FILENO, newLine, 1);
	}
	free(oldPwd);
	free(pwd);
	free(dirInfo);
	return (0);
}

/**
 * shellby_help - Displays information about shellby builtin commands.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		helpAll();
	else if (_strcmp(args[0], "alias") == 0)
		helpAlias();
	else if (_strcmp(args[0], "cd") == 0)
		helpCd();
	else if (_strcmp(args[0], "exit") == 0)
		helpExit();
	else if (_strcmp(args[0], "env") == 0)
		helpEnv();
	else if (_strcmp(args[0], "setenv") == 0)
		helpSetenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		helpUnsetenv();
	else if (_strcmp(args[0], "help") == 0)
		helpHelp();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}

