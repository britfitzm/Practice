#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64

extern char **environ;

void execute_command(char *command);

int main(void)
{
    char *command = NULL;
    size_t bufsize = 0;
    ssize_t bytes_read;

    int is_tty = isatty(STDIN_FILENO);

    while (1)
    {
        if (is_tty)
        {
            write(STDOUT_FILENO, "$ ", 2);
        }

        bytes_read = getline(&command, &bufsize, stdin);
        if (bytes_read <= 0)
        {
            break;
        }

        if (command[bytes_read - 1] == '\n')
        {
            command[bytes_read - 1] = '\0';
        }

        execute_command(command);
    }

    free(command);

    return 0;
}

void execute_command(char *command)
{
    int status;
    pid_t pid;

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        char *args[MAX_ARGS];
        int arg_count = 0;
        char *token;
        char *slash_pos;
        char *path;
        char path_copy[MAX_BUFFER];
        char command_path[MAX_BUFFER];
        char *dir;

        token = strtok(command, " ");
        while (token != NULL)
        {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        slash_pos = strchr(args[0], '/');
        if (slash_pos != NULL)
        {
            execve(args[0], args, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }

        path = getenv("PATH");
        strncpy(path_copy, path, sizeof(path_copy));

        dir = strtok(path_copy, ":");
        while (dir != NULL)
        {
            snprintf(command_path, sizeof(command_path), "%s/%s", dir, args[0]);
            execve(command_path, args, environ);
            dir = strtok(NULL, ":");
        }

        perror("command not found");
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}

