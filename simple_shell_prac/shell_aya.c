#include "shell.h"

/**
 * main - program mimics a shell that read and executes commands
 * @argc: argument counts
 * @argv: argument vector
 * Return: 0 (success)
 */

int main(int argc, char *argv[])
{
	char *prompt = "#cisfun$ ";
	char *lineptr = NULL;
	char *lineptr2 = NULL; /* for getline */
	size_t n = 0; /* for getline */
	ssize_t var;
	const char *delim = " \n";
	char *token;
	int token_count = 0;
	int i; /*loop*/

	do {
		printf("%s", prompt);
		var = getline(&lineptr, &n, stdin);

		if (var == -1)
		{
			return (-1);
		}
		
		lineptr2 = malloc(sizeof(char) * var);

		if (lineptr2 == NULL)
		{
			perror("Error: Cannot allocate memory");
			return (-1);
		}

		strcpy(lineptr2, lineptr);

		token = strtok(lineptr, delim);

		while (token != NULL)
		{
			token_count++;
			token = strtok(NULL, delim);
		}
		token_count++;

		argv = malloc(sizeof(char *) * token_count);

		token = strtok(lineptr2, delim);

		for (i = 0; token != NULL; i++)
		{
			argv[i] = malloc(sizeof(char) * strlen(token));
			strcpy(argv[i], token);

			token = strtok(NULL, delim);
		}
		argv[i] = NULL;

		_execve(argv);

	} while (1);

	free(lineptr);
	free(lineptr2);

	return (0);
}
