#include "shell.h"
#include <stdlib.h>

char *generate_env_error(char **arguments);
char *generate_error_1(char **arguments);
char *generate_error_2_exit(char **arguments);
char *generate_error_2_cd(char **arguments);
char *generate_error_2_syntax(char **arguments);

/**
 * generate_env_error - Creates an error message for shellby_env errors.
 * @arguments: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *generate_env_error(char **arguments)
{
	char *error, *hist_str;
	int len;

	hist_str = convert_to_string(hist);
	if (!hist_str)
		return (NULL);

	arguments--;
	len = _strlen(shell_name) + _strlen(hist_str) + _strlen(arguments[0]) + 45;
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
	_strcat(error, ": Unable to add/remove from environment\n");

	free(hist_str);
	return (error);
}

/**
 * generate_error_1 - Creates an error message for shellby_alias errors.
 * @arguments: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *generate_error_1(char **arguments)
{
	char *error;
	int len;

	len = _strlen(shell_name) + _strlen(arguments[0]) + 13;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
		return (NULL);

	_strcpy(error, "alias: ");
	_strcat(error, arguments[0]);
	_strcat(error, " not found\n");

	return (error);
}

/**
 * generate_error_2_exit - Creates an error message for shellby_exit errors.
 * @arguments: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *generate_error_2_exit(char **arguments)
{
	char *error, *hist_str;
	int len;

	hist_str = convert_to_string(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(shell_name) + _strlen(hist_str) + _strlen(arguments[0]) + 27;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, shell_name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": exit: Illegal number: ");
	_strcat(error, arguments[0]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * generate_error_2_cd - Creates an error message for shellby_cd errors.
 * @arguments: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *generate_error_2_cd(char **arguments)
{
	char *error, *hist_str;
	int len;

	hist_str = convert_to_string(hist);
	if (!hist_str)
		return (NULL);

	if (arguments[0][0] == '-')
		arguments[0][2] = '\0';
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
	if (arguments[0][0] == '-')
		_strcat(error, ": cd: Illegal option ");
	else
		_strcat(error, ": cd: can't cd to ");
	_strcat(error, arguments[0]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * generate_error_2_syntax - Creates an error message for syntax errors.
 * @arguments: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *generate_error_2_syntax(char **arguments)
{
	char *error, *hist_str;
	int len;

	hist_str = convert_to_string(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(shell_name) + _strlen(hist_str) + _strlen(arguments[0]) + 33;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, shell_name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Syntax error: \"");
	_strcat(error, arguments[0]);
	_strcat(error, "\" unexpected\n");

	free(hist_str);
	return (error);
}

