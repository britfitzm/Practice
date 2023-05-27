#include "shell.h"
#include <stdlib.h>

char *generate_error_126(char **arguments);
char *generate_error_127(char **arguments);

/**
 * generate_error_126 - Creates an error message for permission denied failures.
 * @arguments: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *generate_error_126(char **arguments)
{
	char *error, *hist_str;
	int len;

	hist_str = convert_to_string(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(shell_name) + _strlen(hist_str) + _strlen(arguments[0]) + 24;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, shell_name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, arguments[0]);
	_strcat(error, ": Permission denied\n");

	free(hist_str);
	return (error);
}

/**
 * generate_error_127 - Creates an error message for command not found failures.
 * @arguments: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *generate_error_127(char **arguments)
{
	char *error, *hist_str;
	int len;

	hist_str = convert_to_string(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(shell_name) + _strlen(hist_str) + _strlen(arguments[0]) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, shell_name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, arguments[0]);
	_strcat(error, ": not found\n");

	free(hist_str);
	return (error);
}

