#include "shell.h"

/**
 * main - function prints a prompt for a command
 * prompt will be displayed again after command execution
 * @argc: argument count
 * @argv: argument variable
 * Return: pid
 */

int main(int argc, char *argv[])
{
	char *prompt = "$ ";
	char *str = NULL;
	size_t n = 0;
	ssize_t var; /*lets use an unsiged in instedd */

	do {
		write(STDOUT_FILENO, prompt, strlen(prompt)); /* instead of printf */
		var = getline(&str, &n, stdin);

		if (var == -1)
		{
			break;
		}
	} while (1);
	free(str); /* freed the memory from getline */
	pid_t pid;
	char *filename = "bin/ls";
	char *cmd_argv[] = {filename, "-1", NULL};

	pid = fork();

	if (pid == -1)
	{
	perror("fork");
	return (-1);
	}

	if (pid == 0) /* cd process, fork)*/
	{
		int val = execve(filename, argv, NULL);

		if (val == -1)
		perror("evecve");
		return (-1);
	}
	else /* pp from from fork */
	{
		waitpid(pid, NULL, 0);	/* cp to finsh */
		write(STDOUT_FILENO, "\n", 1); /* lets used write */
	}
	return (0);
}
