#include "shell.h"
#include <stdlib.h>

char **copy_environment(void);
void free_environment(void);
char **get_environment(const char *var);

/**
 * copy_environment - Creates a copy of the environment.
 *
 * Return: If an error occurs - NULL.
 *         O/w - a double pointer to the new copy.
 */
char **copy_environment(void)
{
	char **new_environment;
	size_t size;
	int index;

	for (size = 0; environ[size]; size++)
		;

	new_environment = malloc(sizeof(char *) * (size + 1));
	if (!new_environment)
		return (NULL);

	for (index = 0; environ[index]; index++)
	{
		new_environment[index] = malloc(_strlen(environ[index]) + 1);

		if (!new_environment[index])
		{
			for (index--; index >= 0; index--)
				free(new_environment[index]);
			free(new_environment);
			return (NULL);
		}
		_strcpy(new_environment[index], environ[index]);
	}
	new_environment[index] = NULL;

	return (new_environment);
}

/**
 * free_environment - Frees the environment copy.
 */
void free_environment(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * get_environment - Gets an environmental variable from the PATH.
 * @var: The name of the environmental variable to get.
 *
 * Return: If the environmental variable does not exist - NULL.
 *         Otherwise - a pointer to the environmental variable.
 */
char **get_environment(const char *var)
{
	int index, len;

	len = _strlen(var);
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], len) == 0)
			return (&environ[index]);
	}

	return (NULL);
}

