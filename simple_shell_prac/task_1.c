#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * shell_exec - executes commands from command prompt
 * Return: pid
 */

int shell_exec(void)
{
	char *prompt = "$ ";
	size_t n = 10;
	char *buff = malloc(sizeof(char) * n);

	printf("%s", prompt);
	getline(&buff, &n, stdin);

	pid_t pid;

	char *argv[] = {"/bin", "-l", NULL};

	pid = fork();

	if (pid == -1)
		return (-1);

	if (pid == 0)
	{
		int val = execve(argv[0], argv, NULL);

		if (val == -1)
			perror("Not a command\n");
	}
	else
	{
		wait(NULL);
		printf("\n");
	}

	free(buff);

	return (0);
}
