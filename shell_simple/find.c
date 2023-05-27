#include "shell.h"

char *getFullPathDir(char *path);
list_t *getPathDir(char *path);

/**
 * getLocation - Locates a command in the PATH.
 * @command: The command to locate.
 *
 * Return: If an error occurs or the command cannot be located - NULL.
 *         Otherwise - the full pathname of the command.
 */
char *getLocation(char *command)
{
	char **path, *temp;
	list_t *dirs, *head;
	struct stat st;

	path = getEnvironmentVariable("PATH");
	if (!path || !(*path))
		return (NULL);

	dirs = getPathDir(*path + 5);
	head = dirs;

	while (dirs)
	{
		temp = malloc(strlen(dirs->dir) + strlen(command) + 2);
		if (!temp)
			return (NULL);

		strcpy(temp, dirs->dir);
		strcat(temp, "/");
		strcat(temp, command);

		if (stat(temp, &st) == 0)
		{
			freeList(head);
			return (temp);
		}

		dirs = dirs->next;
		free(temp);
	}

	freeList(head);

	return (NULL);
}

/**
 * fillPathDir - Copies path but also replaces leading/sandwiched/trailing
 *               colons (:) with current working directory.
 * @path: The colon-separated list of directories.
 *
 * Return: A copy of path with any leading/sandwiched/trailing colons replaced
 *         with the current working directory.
 */
char *fillPathDir(char *path)
{
	int i, length = 0;
	char *pathCopy, *pwd;

	pwd = *getEnvironmentVariable("PWD") + 4;
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (path[i + 1] == ':' || i == 0 || path[i + 1] == '\0')
				length += strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	pathCopy = malloc(sizeof(char) * (length + 1));
	if (!pathCopy)
		return (NULL);
	pathCopy[0] = '\0';
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (i == 0)
			{
				strcat(pathCopy, pwd);
				strcat(pathCopy, ":");
			}
			else if (path[i + 1] == ':' || path[i + 1] == '\0')
			{
				strcat(pathCopy, ":");
				strcat(pathCopy, pwd);
			}
			else
				strcat(pathCopy, ":");
		}
		else
		{
			strncat(pathCopy, &path[i], 1);
		}
	}
	return (pathCopy);
}

/**
 * getPathDir - Tokenizes a colon-separated list of
 *              directories into a list_t linked list.
 * @path: The colon-separated list of directories.
 *
 * Return: A pointer to the initialized linked list.
 */
list_t *getPathDir(char *path)
{
	int index;
	char **dirs, *pathCopy;
	list_t *head = NULL;

	pathCopy = fillPathDir(path);
	if (!pathCopy)
		return (NULL);
	dirs = strtok(pathCopy, ":");
	free(pathCopy);
	if (!dirs)
		return (NULL);

	for (index = 0; dirs[index]; index++)
	{
		if (addNodeEnd(&head, dirs[index]) == NULL)
		{
			freeList(head);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);

	return (head);
}

