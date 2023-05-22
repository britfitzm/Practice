#include "shell.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void execute_command(const char *command, char **arguments);

int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	char *arguments[MAX_ARGUMENTS];
	char *token;
	int arg_index;

	while (1)
	{
		write(STDOUT_FILENO, "shell $ ", 8);

		if (read(STDIN_FILENO, command, sizeof(command)) <= 0)
		{
			break;
		}

		command[strcspn(command, "\n")] = '\0';

		token = strtok(command, " ");
		arg_index = 0;

		while (token != NULL)
		{
			arguments[arg_index] = token;
			arg_index++;

			if (arg_index >= MAX_ARGUMENTS - 1)
			{
				break;
			}

			token = strtok(NULL, " ");
		}

		arguments[arg_index] = NULL;

		if (strchr(command, '/') == NULL)
		{
			char *path = getenv("PATH");
			char *dir = strtok(path, ":");

			while (dir != NULL)
			{
				char command_path[MAX_COMMAND_LENGTH];
				int command_path_length = snprintf(command_path, sizeof(command_path), "%s/%s", dir, command);

				if (command_path_length >= (int)sizeof(command_path))
				{
					fprintf(stderr, "Command path is too long.\n");
					break;
				}

				if (access(command_path, X_OK) == 0)
				{
					execute_command(command_path, arguments);
					break;
				}

				dir = strtok(NULL, ":");
			}
		}
		else
		{
			execute_command(command, arguments);
		}
	}

	return 0;
}

void execute_command(const char *command, char **arguments)
{
	pid_t child_pid = fork();

	if (child_pid == 0)
	{
		execvp(command, arguments);
		exit(EXIT_FAILURE);
	}
	else if (child_pid > 0)
	{
		int status;
		waitpid(child_pid, &status, 0);
	}
	else
	{
		perror("Fork failed");
	}
}

