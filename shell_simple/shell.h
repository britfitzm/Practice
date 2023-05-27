#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

/* String Manipulation Functions */
int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);
char *string_concatenate(char *dest, const char *src);

/* Custom I/O Functions */
ssize_t custom_getline(char **line_buffer, size_t *buffer_size, int fd);

/* Other Functions */
ssize_t get_new_length(char *line);
void logical_operators(char *line, ssize_t *new_length);
char **replaceAliases(char **arguments);
int custom_strncmp(const char *s1, const char *s2, size_t n);
void replaceVariables(char **commandLine, int exitStatus);
void processLine(char **commandLine, ssize_t bytesRead);
int (*getBuiltin(const char *command))(char **arguments, char **env);
int custom_execute(char **arguments, char **env);
void custom_free_args(char **args1, char **args2);
char **custom_strtok(char *str, const char *delimiters);
char *custom_create_error(const char *command, int error_num);

#endif /* SHELL_H */

