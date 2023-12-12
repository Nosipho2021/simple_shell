#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * main main - implements simple shell
 * Return: Always 0
 * Description: main main is a function that implements simple shell
 */

int main(void)
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		printf("$");
		ffush(stdout);

	if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
	{
		break;
	}

	command[strcspn(command, "\n")] = '\0';

	if (execlp(command, command, (char *) NULL) == -1)
	{
		printf("Error: command not found\n");
	}

	return (0);
}

