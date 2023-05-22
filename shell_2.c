#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * shell_exec - executes commands from the command prompt
 * Return: pid
 */
int shell_exec(void)
{
    pid_t pid;
    char *prompt = "$ ";
    size_t n = 10;
    char *buff = malloc(sizeof(char) * n);
    char **argv = malloc(sizeof(char *) * 3);
    char *token;

    write(STDOUT_FILENO, prompt, strlen(prompt));

    if (getline(&buff, &n, stdin) == -1)
    {
        perror("Failed to read input\n");
        return (-1);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("Fork failed\n");
        return (-1);
    }

    if (pid == 0)
    {
        int i = 0;
        token = strtok(buff, " \n");
        while (token != NULL)
        {
            argv[i++] = token;
            token = strtok(NULL, " \n");
        }
        argv[i] = NULL;

        execve(argv[0], argv, NULL);
        perror("Command execution failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        return (status);
    }
}
int main(void)
{
	shell_exec();

	return (0);
}
