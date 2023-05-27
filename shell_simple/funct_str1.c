#include "shell.h"

/**
 * string_length - Returns the length of a string.
 * @s: A pointer to the string.
 *
 * Return: The length of the string.
 */
int string_length(const char *s)
{
	int length = 0;

	if (!s)
		return (length);

	while (s[length])
		length++;

	return (length);
}

/**
 * string_copy - Copies the string pointed to by src, including the
 *               terminating null byte, to the buffer pointed by dest.
 * @dest: Pointer to the destination buffer.
 * @src: Pointer to the source string.
 *
 * Return: Pointer to dest.
 */
char *string_copy(char *dest, const char *src)
{
	int i;

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];

	dest[i] = '\0';

	return (dest);
}

/**
 * string_concatenate - Concatenates two strings.
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string.
 *
 * Return: Pointer to the destination string.
 */
char *string_concatenate(char *dest, const char *src)
{
	char *dest_temp = dest;
	const char *src_temp = src;

	while (*dest_temp != '\0')
		dest_temp++;

	while (*src_temp != '\0')
		*dest_temp++ = *src_temp++;

	*dest_temp = '\0';

	return (dest);
}

