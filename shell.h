#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

/* Function prototypes */
int shell_exec(void);
int _execve(char *argv[]);
void execute_command(const char *command, char **arguments); // Updated return type
void handle_signal(int signal);
char *get_input(void);
char **parse_input(char *input);
char **get_directories(char *path);
char *get_command_path(char *command, char **directories);
int count_directories(char *path);
void free_array(char **array);
void free_directories(char **directories);
#endif
