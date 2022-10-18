/* 
Assignment 1 Question 2a
*/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *filename[])
{
	char text[1048576];
	char copy[1048576];
	char* sourceFile = filename[1];
	char* destinationFile = filename[2];

	int fd[2];
	pid_t pid;

	/* Creating a pipe*/
	pipe(fd);
	if(pipe(fd)==-1)
	{
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
	
	/* Fork a child process*/
	pid = fork();

	if(pid<0)	//Invalid
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	if(pid>0)	//Parent process
	{
		int sourceFileNum;
		ssize_t nBytes;

		close(fd[READ_END]);	//Close the unused end of the pipe

		sourceFileNum = open(sourceFile, O_RDONLY);
		nBytes = read(sourceFileNum, text, sizeof(text));

		write(fd[WRITE_END], text, nBytes);	//Write to the pipe

		close(fd[WRITE_END]);	//Close the write end of the pipe
	}

	else	//Child process
	{
		int destinationFiledesc;
		ssize_t nBytes;

		close(fd[WRITE_END]);	//Close the unused end of the pipe

		destinationFiledesc = open(destinationFile, O_CREAT | O_WRONLY);
		nBytes = read(fd[READ_END], copy, sizeof(copy));

		write(destinationFiledesc, copy, nBytes);	//Write to the destination file

		close(fd[READ_END]);
	}

	return 0;
}
