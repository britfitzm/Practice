#include "shell.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void *custom_reallocate(void *pointer, unsigned int old_size, unsigned int new_size);
void assign_line_buffer(char **line_buffer, size_t *buffer_size, char *buffer, size_t size);
ssize_t custom_getline(char **line_buffer, size_t *buffer_size, FILE *stream);

/**
 * custom_reallocate - Reallocates a memory block using malloc and free.
 * @pointer: A pointer to the memory previously allocated.
 * @old_size: The size in bytes of the allocated space for pointer.
 * @new_size: The size in bytes for the new memory block.
 *
 * Return: If new_size == old_size, return pointer.
 *         If new_size == 0 and pointer is not NULL, return NULL.
 *         Otherwise, return a pointer to the reallocated memory block.
 */
void *custom_reallocate(void *pointer, unsigned int old_size, unsigned int new_size)
{
	void *memory;
	char *ptr_copy, *filler;
	unsigned int index;

	if (new_size == old_size)
		return (pointer);

	if (pointer == NULL)
	{
		memory = malloc(new_size);
		if (memory == NULL)
			return (NULL);

		return (memory);
	}

	if (new_size == 0 && pointer != NULL)
	{
		free(pointer);
		return (NULL);
	}

	ptr_copy = pointer;
	memory = malloc(sizeof(*ptr_copy) * new_size);
	if (memory == NULL)
	{
		free(pointer);
		return (NULL);
	}

	filler = memory;

	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = (*ptr_copy)++;

	free(pointer);
	return (memory);
}

/**
 * assign_line_buffer - Reassigns the line_buffer variable for custom_getline.
 * @line_buffer: A buffer to store an input string.
 * @buffer_size: The size of line_buffer.
 * @buffer: The string to assign to line_buffer.
 * @size: The size of buffer.
 */
void assign_line_buffer(char **line_buffer, size_t *buffer_size, char *buffer, size_t size)
{
	if (*line_buffer == NULL)
	{
		if (size > 120)
			*buffer_size = size;
		else
			*buffer_size = 120;
		*line_buffer = buffer;
	}
	else if (*buffer_size < size)
	{
		if (size > 120)
			*buffer_size = size;
		else
			*buffer_size = 120;
		*line_buffer = buffer;
	}
	else
	{
		strcpy(*line_buffer, buffer);
		free(buffer);
	}
}

/**
 * custom_getline - Reads input from a stream.
 * @line_buffer: A buffer to store the input.
 * @buffer_size: The size of line_buffer.
 * @stream: The stream to read from.
 *
 * Return: The number of bytes read.
 */
ssize_t custom_getline(char **line_buffer, size_t *buffer_size, FILE *stream)
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
			buffer = custom_reallocate(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	assign_line_buffer(line_buffer, buffer_size, buffer, input);

	ret = input;
	if (r != 0)
		input = 0;
	return (ret);
}

