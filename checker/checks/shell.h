#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *command);
void handle_arguments(char *command);
void handle_PATH(char **args);
void shell_exit(void);
void handle_env(void);
char **parse_arguments(char *command);
void free_arguments(char **args);
int handle_builtin(char **args);
void execute_external_command(char **args);

extern char **environ;

#endif
