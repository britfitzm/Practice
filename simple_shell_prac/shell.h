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

int shell_exec(void);
int _execve(char *argv[]);

#endif
