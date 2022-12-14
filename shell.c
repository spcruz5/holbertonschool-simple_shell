#include "main.h"
/**
 * main - simple shell
 * @ac: counter
 * @av: arguments
 * @env: enviorment
 * Return: 0
 */
int main(int ac, char **av, char **env)
{
	char *lineptr = NULL, **PATH = NULL, *DIRE, *CONCAT, **str;
	size_t n = 0;
	pid_t child_pid;
	int status;
	struct stat buf;
	(void)ac;
	(void)av;

	while (1)
	{
		if (isatty(0))
			printf("$ ");
		if (getline(&lineptr, &n, stdin) == -1)
			break;
		if (_strcmp(lineptr, "exit\n") == 0)
		{
			free(lineptr);
			lineptr = NULL;
			exit(0);
		}
		if (_strcmp(lineptr, "env\n") == 0)
		{
			free(lineptr);
			lineptr = NULL;
			print_env();
			exit(0);
		}
		str = split_line(lineptr);
		if (str[0] != NULL)
		{
			if (stat(str[0], &buf) == -1)
			{
				PATH = get_path(env);
				DIRE = getDir(PATH, str);
				if (DIRE != NULL)
					CONCAT = concat_str_dir(DIRE, str[0]);
			}
			else
				CONCAT = str[0];

			child_pid = fork();
			if (child_pid != 0)
			{
				wait(&status);
				free(str);
				str = NULL;
			}
			else
			{
				if (execve(CONCAT, str, env))
				{
					if (stat(str[0], &buf) == -1)
						free_fun(PATH);
					perror("execve");
					exit(EXIT_FAILURE);
				}
			}
		}
		free_fun(PATH);
	}
	free(str);
	str = NULL;
	free(lineptr);
	lineptr = NULL;
	return (0);
}
