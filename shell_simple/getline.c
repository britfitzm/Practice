#include "shell.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void assign_line_buffer(char **linebuf, size_t *bufsize, char *buf, size_t b);
ssize_t custom_getline(char **linebuf, size_t *bufsize, FILE *stream);

/**
 * custom_realloc - Reallocates a memory block using malloc and free.
 * @ptr: A pointer to the memory previously allocated.
 * @old_size: The size in bytes of the allocated space for ptr.
 * @new_size: The size in bytes for the new memory block.
 *
 * Return: If new_size == old_size - ptr.
 *         If new_size == 0 and ptr is not NULL - NULL.
 *         Otherwise - a pointer to the reallocated memory block.
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *mem;
	char *ptrcpy, *filler;
	unsigned int idx;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		mem = malloc(new_size);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptrcpy = ptr;
	mem = malloc(sizeof(*ptrcpy) * new_size);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mem;

	for (idx = 0; idx < old_size && idx < new_size; idx++)
		filler[idx] = *ptrcpy++;

	free(ptr);
	return (mem);
}

/**
 * assign_line_buffer - Reassigns the linebuf variable for custom_getline.
 * @linebuf: A buffer to store an input string.
 * @bufsize: The size of linebuf.
 * @buf: The string to assign to linebuf.
 * @b: The size of buf.
 */
void assign_line_buffer(char **linebuf, size_t *bufsize, char *buf, size_t b)
{
	if (*linebuf == NULL)
	{
		if (b > 120)
			*bufsize = b;
		else
			*bufsize = 120;
		*linebuf = buf;
	}
	else if (*bufsize < b)
	{
		if (b > 120)
			*bufsize = b;
		else
			*bufsize = 120;
		*linebuf = buf;
	}
	else
	{
		strcpy(*linebuf, buf);
		free(buf);
	}
}

/**
 * custom_getline - Reads input from a stream.
 * @linebuf: A buffer to store the input.
 * @bufsize: The size of linebuf.
 * @stream: The stream to read from.
 *
 * Return: The number of bytes read.
 */
ssize_t custom_getline(char **linebuf, size_t *bufsize, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int r;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (r == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = custom_realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	assign_line_buffer(linebuf, bufsize, buffer, input);

	ret = input;
	if (r != 0)
		input = 0;
	return (ret);
}

