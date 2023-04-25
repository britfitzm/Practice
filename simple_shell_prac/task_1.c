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
	char *str;
	size_t n = 0;
	int var; /* this holds the getline function */

	do {
		printf("%s", prompt);
		var = getline(&str, &n, stdin);

		if (var == -1)
		{
			break;
		}
	} while (1);

	pid_t pid;

	char *argv[] = {"/usr/bin", "-l", NULL};

	if (pid == -1)
		return (-1);

	if (pid == 0)
	{
		int val = execve(filename, argv, NULL);

		if (val == -1)
			perror("Not Command\n");
	}
	else
	{
		printf("\n");
	}

	return (0);
}
