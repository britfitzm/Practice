#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * shell_exec - executes commands from command prompt
 * Return: 0 on success, -1 on failure
 */
int shell_exec(void)
{
	char *prompt = "$ ";
	size_t n = 10;
	char *buff = malloc(sizeof(char) * n);
	pid_t pid;
	int status;

	write(1, prompt, 2); /* Use write instead of printf */

	getline(&buff, &n, stdin);

	char *argv[] = {"/bin/ls", "-l", NULL}; /* Update the command line arguments */

	pid = fork();

	if (pid == -1) {
		write(2, "fork\n", 5);
		return (-1);
	}

	if (pid == 0) {
		int val = execve(argv[0], argv, NULL);

		if (val == -1) {
			char *error_message = "No such file or directory\n";
			write(2, error_message, 27); /* Update the error message */
		}

		exit(1); /* Terminate the child process after execve */
	}

	if (waitpid(pid, &status, 0) == -1) {
		write(2, "waitpid\n", 8);
		return (-1);
	}

	/* Handle the exit status of the child process if necessary */

	return (0);
}

