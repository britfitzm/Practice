#include "shell.h"
#include <unistd.h>  // Include the necessary header for the write function

int execute_alias(char **arguments, char __attribute__((__unused__)) **front);
void set_alias(char *alias_name, char *alias_value);
void print_alias(alias_t *alias);

/**
 * execute_alias - Builtin command that either prints all aliases, specific
 * aliases, or sets an alias.
 * @arguments: An array of arguments.
 * @front: A double pointer to the beginning of arguments.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int execute_alias(char **arguments, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	int i, ret = 0;
	char *value;

	if (!arguments[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return ret;
	}
	for (i = 0; arguments[i]; i++)
	{
		temp = aliases;
		value = _strchr(arguments[i], '=');
		if (!value)
		{
			while (temp)
			{
				if (_strcmp(arguments[i], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				ret = create_error(arguments + i, 1);
		}
		else
			set_alias(arguments[i], value);
	}
	return ret;
}

/**
 * set_alias - Will either set an existing alias 'name' with a new value,
 * 'value' or creates a new alias with 'name' and 'value'.
 * @alias_name: Name of the alias.
 * @alias_value: Value of the alias. First character is a '='.
 */
void set_alias(char *alias_name, char *alias_value)
{
	alias_t *temp = aliases;
	int len, j, k;
	char *new_value;

	*alias_value = '\0';
	alias_value++;
	len = _strlen(alias_value) - _strspn(alias_value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return;
	for (j = 0, k = 0; alias_value[j]; j++)
	{
		if (alias_value[j] != '\'' && alias_value[j] != '"')
			new_value[k++] = alias_value[j];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (_strcmp(alias_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		add_alias_end(&aliases, alias_name, new_value);
}

/**
 * print_alias - Prints the alias in the format name='value'.
 * @alias: Pointer to an alias.
 */
void print_alias(alias_t *alias)
{
	char *alias_string;
	int len = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (len + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, len);  // Use write function to print the alias
	free(alias_string);
}

/**
 * replace_aliases - Goes through the arguments and replace any matching alias
 * with their value.
 * @arguments: 2D pointer to the arguments.
 *
 * Return: 2D pointer to the arguments.
 */
char **replace_aliases(char **arguments)
{
	alias_t *temp;
	int i;
	char *new_value;

	if (_strcmp(arguments[0], "alias") == 0)
		return arguments;
	for (i = 0; arguments[i]; i++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(arguments[i], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (_strlen(temp->value) + 1));
				if (!new_value)
				{
					free_args(arguments, arguments);
					return NULL;
				}
				_strcpy(new_value, temp->value);
				free(arguments[i]);
				arguments[i] = new_value;
				i--;
				break;
			}
			temp = temp->next;
		}
	}

	return (arguments);
}

