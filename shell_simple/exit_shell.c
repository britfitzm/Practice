#include "shell.h"
#include <stdlib.h>

int calculate_num_len(int number);
char *convert_to_string(int number);
int generate_error_message(char **arguments, int error);

/**
 * calculate_num_len - Counts the digit length of a number.
 * @number: The number to measure.
 *
 * Return: The digit length.
 */
int calculate_num_len(int number)
{
	unsigned int num1;
	int length = 1;

	if (number < 0)
	{
		length++;
		num1 = number * -1;
	}
	else
	{
		num1 = number;
	}
	while (num1 > 9)
	{
		length++;
		num1 /= 10;
	}

	return (length);
}

/**
 * convert_to_string - Converts an integer to a string.
 * @number: The integer.
 *
 * Return: The converted string.
 */
char *convert_to_string(int number)
{
	char *buffer;
	int length = calculate_num_len(number);
	unsigned int num1;

	buffer = malloc(sizeof(char) * (length + 1));
	if (!buffer)
		return (NULL);

	buffer[length] = '\0';

	if (number < 0)
	{
		num1 = number * -1;
		buffer[0] = '-';
	}
	else
	{
		num1 = number;
	}

	length--;
	do {
		buffer[length] = (num1 % 10) + '0';
		num1 /= 10;
		length--;
	} while (num1 > 0);

	return (buffer);
}


/**
 * generate_error_message - Writes a custom error message to stderr.
 * @arguments: An array of arguments.
 * @error: The error value.
 *
 * Return: The error value.
 */
int generate_error_message(char **arguments, int error)
{
	char *error_message;

	switch (error)
	{
	case -1:
		error_message = create_env_error(arguments);
		break;
	case 1:
		error_message = create_error_1(arguments);
		break;
	case 2:
		if (*(arguments[0]) == 'e')
			error_message = create_error_2_exit(++arguments);
		else if (arguments[0][0] == ';' || arguments[0][0] == '&' || arguments[0][0] == '|')
			error_message = create_error_2_syntax(arguments);
		else
			error_message = create_error_2_cd(arguments);
		break;
	case 126:
		error_message = create_error_126(arguments);
		break;
	case 127:
		error_message = create_error_127(arguments);
		break;
	}
	write(STDERR_FILENO, error_message, _strlen(error_message));

	if (error_message)
		free(error_message);
	return (error);
}

