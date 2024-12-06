/*

Program to show on waitpid()

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
	pid_t cpid1, cpid2,w;
	int status;

	cpid1 = fork();
	cpid2 = fork();

	if(cpid1 == -1)
	{
		printf("\nUnable to create the child process by cpid1\n");
		exit(EXIT_FAILURE);
	}
	else if(cpid2 == 0)
	{
		printf("\n(Child's child)Second child's pid: %d", getpid());
		while(1);
		_exit(EXIT_SUCCESS);

	}
	else if(cpid1 == 0)
	{
		printf("\nFirst child pid: %d", getpid());
		while(1);
		// if(argc == 1)
		// 	pause();
		// _exit(atoi(argv[1]));
	}
	else
	{
		do{
			w = waitpid(cpid2, &status,WUNTRACED|WCONTINUED);
			if(w == -1)
			{
				perror("waitpid() ");
				_exit(EXIT_FAILURE);
			}

			if(WIFEXITED(status))
			{
				printf("\nStatus : %d", WEXITSTATUS(status));
			}
			else if(WIFSIGNALED(status))
			{
				printf("\nKilled by the signal: %d", WTERMSIG(status));
			}
			else if(WIFSTOPPED(status))
			{
				printf("\nStoped by Signal %d",WSTOPSIG(status));
			}
			else if(WIFCONTINUED(status))
			{
				printf("\nContinued");

			}
			else
			{
				printf("\nUnCaught signal");
			}


		}while(!WIFSIGNALED(status) && !WIFEXITED(status));
		exit(EXIT_SUCCESS);
	}

	printf("\n\n");
	return 0;
}