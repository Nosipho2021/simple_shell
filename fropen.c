#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

struct Alias
{
	char *name;
	char *value;
};


struct Alias aliases[MAX_ARGUMENTS];
int num_aliases = 0;

/**
 * find_alias_index - Find the index of an alias by name
 * 
 * @name: The name of the alias
 * 
 * Return: The index of the alias if found, -1 otherwise
 */
int find_alias_index(const char *name)
{
	for (int i = 0; i < num_aliases; i++)
	{
	if (strcmp(aliases[i].name, name) == 0)
	{
	return (i);
        }
	}
	return (-1);
}

/**
 * print_aliases - Print aliases based on user input
 * 
 * @args: Array of alias names or NULL for all aliases
 */
void print_aliases(char **args)
{
    if (args == NULL) {
        // Print all aliases
	for (int i = 0; i < num_aliases; i++) {
	printf("%s='%s'\n", aliases[i].name, aliases[i].value);
	}
    }
    else
    {
        // Print specific aliases
	for (int i = 0; args[i] != NULL; i++)
	{
	int index = find_alias_index(args[i]);
	if (index != -1)
	{
	printf("%s='%s'\n", aliases[index].name, aliases[index].value);
	}
	}
	}
}

/**
 * set_alias - Set or replace an alias
 * 
 * @name: The name of the alias
 * @value: The value of the alias
 */
void set_alias(const char *name, const char *value)
{
	int index = find_alias_index(name);
	if (index != -1) {
        // If the alias already exists, replace its value
	free(aliases[index].value);
	aliases[index].value = strdup(value);
	} 
	else
	{
        // If the alias doesn't exist, create a new one
	if (num_aliases < MAX_ARGUMENTS)
	{
	aliases[num_aliases].name = strdup(name);
	aliases[num_aliases].value = strdup(value);
	num_aliases++;
        }
	else 
	{
		fprintf(stderr, "Error: Maximum number of aliases reached\n");		}
	}
}

/**
 * replace_variables - Replace variables in the command with their values
 * 
 * @command: The command to be executed
 * 
 * Return: The modified command with variable values
 */
char *replace_variables(char *command)
{
	char *result = malloc(strlen(command) + 1);
	if (!result)
	{
	perror("malloc");
	exit(EXIT_FAILURE);
	}

	char *p = command;
	char *r = result;
	while (*p)
	{
	if (*p == '$')
	{
        
	p++;
	if (*p == '?')
	{
	int status = 0; 
	if (WIFEXITED(status))
	{
	sprintf(r, "%d", WEXITSTATUS(status));
	}
	else
	{
		sprintf(r, "%d", status);
	}
	r += strlen(r);
	}
       	else if (*p == '$')
       	{
	sprintf(r, "%d", getpid());
	r += strlen(r);
	}
	else 
	{
        char variable[MAX_COMMAND_LENGTH];
	int i = 0;
 	while (*p && (*p == '_' || (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') ||
	(*p >= '0' && *p <= '9'))) {
	variable[i++] = *p++;
		}
	variable[i] = '\0';

        
	char *value = getenv(variable);
		if (value)
	{
	sprintf(r, "%s", value);
	r += strlen(r);
	}
	       	else
	{
                    
	strncpy(r, variable, i);
	r += i;
	}
	}
	}
	else
	{
	*r++ = *p++;
	}
	}

	*r = '\0';
	return (result);
}

/**
 * execute_command - Execute a command with arguments
 * 
 * @command: The command to be executed
 * 
 * Return: 1 if the shell should continue, 0 if the shell should exit
 */
int execute_command(char *command)
{
    	
	if (command[0] == '\n' || command[0] == '#')
	{
        return (1);
	}

	char *commands[MAX_COMMAND_LENGTH];
	int command_index = 0;

	char *args[MAX_ARGUMENTS];
	int arg_index = 0;

	int i;
	for (i = 0; i < MAX_COMMAND_LENGTH && command[i] != '\n'; ++i)
       	{
        
	while (command[i] == ' ' || command[i] == '\t')
	{
	i++;
	}

        
	args[arg_index] = command + i;
	arg_index++;



