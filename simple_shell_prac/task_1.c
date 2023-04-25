#include "shell.h"

/**
 * main - function prints a prompt for a command
 * prompt will be displayed again after command execution
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


	return (0);
}
