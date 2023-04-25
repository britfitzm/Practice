#include "shell.h"

/**
 * _execve - function executes command
 * @argv: argument vector
 */

int _execve(char *argv[])
{
	char *filename = NULL;
	int e;

	if (argv != NULL)
	{
		filename = argv[0];

		e = execve(filename, argv, NULL);

		if (e == -1)
		{
			perror("Error");
		}
	}
}
