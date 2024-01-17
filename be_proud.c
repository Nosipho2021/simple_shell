#include "shell.h"

/**
 * main - Entry point for the shell program
 *
 * Return: Always 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;

	while (1)
	{
		printf("($) ");
		getline(&line, &len, stdin);

	if (strcmp(line, "exit\n") == 0)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}
	}

	return (0);
}

