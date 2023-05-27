#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * handle_sigint - Handles the SIGINT signal (Ctrl+C)
 *
 * signum: The signal number
 *
 * Description: Handles the SIGINT signal (Ctrl+C) and prints a new prompt.
 */
void handle_sigint(int signum)
{
	(void)signum;
	write(1, "\n$ ", 3);
	fflush(stdout);
}

/**
 * main - Entry point of the shell program
 *
 * Description: Reads user input, executes the command if it starts with "/bin/",
 *              and displays the result.
 * Return: Always 0
 */
int main(void)
{
	char *command = NULL;
	size_t bufsize = 0;
	pid_t child_pid;
	int status;

	signal(SIGINT, handle_sigint);

	while (1)
	{
		write(1, "$ ", 2);

		if (getline(&command, &bufsize, stdin) == -1)
		{
			if (feof(stdin))
				write(1, "\n", 1);
			break;
		}

		command[strcspn(command, "\n")] = '\0';

		if (strncmp(command, "/bin/", 5) != 0)
		{
			write(1, "Command not found: ", 19);
			write(1, command, strlen(command));
			write(1, "\n", 1);
			continue;
		}

		child_pid = fork();
		if (child_pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			char *argv[2];
			argv[0] = command;
			argv[1] = NULL;
			if (execve(command, argv, NULL) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(&status);
		}
	}

	free(command);

	return (0);
}

