#include "shell.h"

alias_t *addAliasEnd(alias_t **aliasHead, char *aliasName, char *aliasValue);
void freeAliasList(alias_t *aliasHead);
list_t *addNodeEnd(list_t **listHead, char *directory);
void freeList(list_t *listHead);

/**
 * addAliasEnd - Adds a node to the end of an alias_t linked list.
 * @aliasHead: A pointer to the head of the alias_t list.
 * @aliasName: The name of the new alias to be added.
 * @aliasValue: The value of the new alias to be added.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */
alias_t *addAliasEnd(alias_t **aliasHead, char *aliasName, char *aliasValue)
{
	alias_t *newNode = malloc(sizeof(alias_t));
	alias_t *last;

	if (!newNode)
		return (NULL);

	newNode->next = NULL;
	newNode->name = malloc(sizeof(char) * (_strlen(aliasName) + 1));
	if (!newNode->name)
	{
		free(newNode);
		return (NULL);
	}
	newNode->value = aliasValue;
	_strcpy(newNode->name, aliasName);

	if (*aliasHead)
	{
		last = *aliasHead;
		while (last->next != NULL)
			last = last->next;
		last->next = newNode;
	}
	else
		*aliasHead = newNode;

	return (newNode);
}

/**
 * addNodeEnd - Adds a node to the end of a list_t linked list.
 * @listHead: A pointer to the head of the list_t list.
 * @directory: The directory path for the new node to contain.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */
list_t *addNodeEnd(list_t **listHead, char *directory)
{
	list_t *newNode = malloc(sizeof(list_t));
	list_t *last;

	if (!newNode)
		return (NULL);

	newNode->dir = directory;
	newNode->next = NULL;

	if (*listHead)
	{
		last = *listHead;
		while (last->next != NULL)
			last = last->next;
		last->next = newNode;
	}
	else
		*listHead = newNode;

	return (newNode);
}

/**
 * freeAliasList - Frees an alias_t linked list.
 * @aliasHead: The head of the alias_t list.
 */
void freeAliasList(alias_t *aliasHead)
{
	alias_t *next;

	while (aliasHead)
	{
		next = aliasHead->next;
		free(aliasHead->name);
		free(aliasHead->value);
		free(aliasHead);
		aliasHead = next;
	}
}

/**
 * freeList - Frees a list_t linked list.
 * @listHead: The head of the list_t list.
 */
void freeList(list_t *listHead)
{
	list_t *next;

	while (listHead)
	{
		next = listHead->next;
		free(listHead->dir);
		free(listHead);
		listHead = next;
	}
}

