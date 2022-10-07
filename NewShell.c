/* 
Assignment 1 - Question 1
This is an Custom Shell created in C
It has 6 custom functions: cd, mkdir, help, touch, echo, exit, which have been implemented using C 
*/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int sh_cd(char **args);
int sh_mkdir(char **args);
int sh_help(char **args);
int sh_exit(char **args);
int sh_touch(char **args);
int sh_echo(char **args);

char *builtin_str[] = {"cd","mkdir","help","touch","echo","exit"};

int (*builtin_func[]) (char **) = {&sh_cd,&sh_mkdir,&sh_help,&sh_touch,&sh_echo,&sh_exit};

int sh_num_builtins() 
{
	return sizeof(builtin_str) / sizeof(char *);
}


int sh_cd(char **args)
{
	if (args[1] == NULL) 
	{
		fprintf(stderr, "Error: expected argument to \"cd\"\n");
	}
	else
	{
		if (chdir(args[1]) != 0) 
		{
			perror("Error");
		}
	}
	return 1;
}

int sh_mkdir(char **args)
{
	if (args[1] == NULL) 
	{
		fprintf(stderr, "Error: expected argument to \"mkdir\"\n");
	}
	else
	{
		if (mkdir(args[1], "write-mode") != 0) 
		{
			perror("Error");
		}
	}
	return 1;
}

int sh_help(char **args)
{
	int i;
	printf("New Shell\n");
	printf("Type program names and arguments, and press enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < sh_num_builtins(); i++)
	{
		printf("  %s\n", builtin_str[i]);
	}
	return 1;
}

int sh_echo(char **args)
{
	if (args[1] == NULL) 
	{
		fprintf(stderr, "Error: expected argument to \"echo\"\n");
	}
	printf(args[1]);
	printf("\n");
	return 1;	
}

int sh_touch(char **args)
{
	if (args[1] == NULL) 
	{
		fprintf(stderr, "Error: expected argument to \"touch\"\n");
	}
	else
	{
		fopen(args[1], "write-mode");
	}
	return 1;
}



int sh_exit(char **args)
{
	return 0;
}


int sh_launch(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)	// Child process
	{
		if (execvp(args[0], args) == -1) 
		{
			perror("sh");
    		}
		exit(EXIT_FAILURE);
  	}
	else if (pid < 0)	// Error in forking
	{
		perror("Error");
	}
	else	// Parent process
	{
		do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int sh_execute(char **args)
{
	int i;

	if (args[0] == NULL)	//Empty command entered
	{
		return 1;
	}

	for (i = 0; i < sh_num_builtins(); i++)
	{	
		if (strcmp(args[0], builtin_str[i]) == 0) 
		{
      			return (*builtin_func[i])(args);
		}
	}

	return sh_launch(args);
}

char *sh_read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0;  //Allocating buffer

	if (getline(&line, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{  
		      exit(EXIT_SUCCESS);  //Recieved EOF
		}
		else
		{
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}
	return line;
}

#define SH_TOK_BUFFERSIZE 64
#define SH_TOK_DELIMITER " \t\r\n\a"

char **sh_split_line(char *line)
{
	int bufsize = SH_TOK_BUFFERSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token, **tokens_backup;

	if (!tokens)
	{
		fprintf(stderr, "sh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, SH_TOK_DELIMITER);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += SH_TOK_BUFFERSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens)
			{
				free(tokens_backup);
        			fprintf(stderr, "sh: allocation error\n");
        			exit(EXIT_FAILURE);
      			}
    		}

    		token = strtok(NULL, SH_TOK_DELIMITER);
  	}
  	tokens[position] = NULL;
  	return tokens;
}

void sh_loop(void)
{
  	char *line;
  	char **args;
  	int status;

  	do 
	{
		printf("NewShell@root> ");
		line = sh_read_line();
		args = sh_split_line(line);
		status = sh_execute(args);

		free(line);
		free(args);
	} while (status);
}

int main(int argc, char **argv)
{
	sh_loop();

	return EXIT_SUCCESS;
}

