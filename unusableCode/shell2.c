//Marc's edit
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

#define COLOR_BLUE	"\x1b[34m"
#define COLOR_RESET	"\x1b[0m" 

/* the current commented out code in regards to piping is a failed attempt to
read from input command, unsure how to fix, if fixed these next 2 lines 
can be commented, currently it pipes command1 into command2, does not return to shell after piping, it seems command2 doesn't close 
*/
char *command1[] = { "/bin/ls", "-a", "/", 0 };
char *command2[] = { "/usr/bin/tr", "a-z", "A-Z", 0 };


void pipeIn(int command[]/*, char* command1[], char* parameters*/)
{
	int pid = fork();
	if (pid  == 0) //child
	{
		dup2(command[1],1);
		close(command[0]);
		int ret = execvp(command1[0], command1);
		if (ret = -1)
		{
		/*	char* otherPath = "/usr/bin";
			strcat(otherPath, parameters);
			strcpy(commands[0], otherPath);
			ret = execvp(commands[0], commands);
			if (ret = -1)
			{*/
				printf("Path not found");
				return;
		//	}
		}	
		close(1);
	}
	if (pid < 0)
	{
		printf("fork error");
		exit(1);
	}
	if (pid > 0)
	{
		wait(NULL);
	}
}

void pipeOut(int command[]/*, char* command2[], char* parameters*/) 
{
	int pid = fork();
	if (pid == 0) //child
	{
		dup2(command[0],0);
		close(command[1]);
		int ret = execvp(command2[0], command2);
		if (ret = -1)
		{
		/*	char* otherPath = "/usr/bin";
			strcat(otherPath, parameters);
			strcpy(commands[0], otherPath);
			ret = execvp(commands[0], commands);
			if (ret = -1)
			{ */
				printf("Path not found");
				return;
		//	}
		}
		close(0);
		close(1);
		close(command[0]);
		close(command[1]);	
}
	if (pid < 0)
        {
                printf("fork error");
                exit(1);
        }
        if (pid > 0)
        {
                wait(NULL);
		close(command[0]);
		close(command[1]);
        }
}

int main(int argc, char* argv[], char* envp[])
{

	printf(COLOR_BLUE"\n //----\\   ||       ||   ||  ||=====     //======  ||    ||  ||====  ||      ||\n");
	printf(" ||    ||  ||       ||   ||  ||          ||        ||    ||  ||      ||      ||\n");
	printf(" ||----    ||       ||   ||  ||=====     \\\\----\\\\  ||====||  ||====  ||      ||\n");
	printf(" ||    ||  ||       ||   ||  ||                ||  ||    ||  ||      ||      ||\n");
	printf(" ||____/   ||=====  \\\\---//  ||=====     ======//  ||    ||  ||====  ||====  ||====\n\n");
	printf(COLOR_RESET);
	bool isPiping = false, isRedirecting = false, isWildcard = false;
	char *pipeSearch, *redirectionSearch, *wildcardSearch;
	int  pipingPosition[256], redirectionPosition[256], wildcardPosition[256];
	char c[256] = "\0";
	char* parameters[256];
	char *cptr;
	int parameterCount = 0;
	char piping = '|', redirection = '>', wildcard  ='*';
        while(1)
	{
		int z;
 		char* path = "/bin/";
		char* envList[] = {"HOME=/root", "PATH=/bin:/sbin", NULL};
		//Loop that resets arrays to null
		for (z=0; z<=256; z++)
		{
			c[z] = '\0';
			parameters[z] = NULL;
		}
		
		//Display the prompt message
		parameterCount = 0;
		printf(COLOR_BLUE"[BLUE_SHELL  ]:- "COLOR_RESET);
		int l;
		 //Loop that reads in input
        	for (l = 0; l <= 256; l++)
		{		
			c[l] = getchar();
			if(c[l] == '\n')
			{
			 	c[l] = '\0';
				break;
			}
		}
        	//Store every word as an array of strings to parse later
		cptr = strtok(c, " ");
		while (cptr != NULL)
		{
			parameters[parameterCount] = cptr;
			parameterCount++;
			cptr = strtok(NULL, " ");	
	 	}
		int i, j = 0, k = 0, m = 0;
		for (i = 0; i < parameterCount; i++)
		{
			pipeSearch = strchr(parameters[i],piping);
			if (pipeSearch!= NULL)
			{
				//pipingPosition[j] = i;
			/*        char* command1[5];// command[0] is the command, everyone after should be the parameters of that command, such -a for ls, or for tr(translate) it would be what you want to translate to, like change to all caps, getting this is the issue  
				char* command2[5];
				command1[0] = path;
				command2[0] = path;
				strcat(command1[0], parameters[pipingPosition[j]-1]);
			        strcat(command2[0], parameters[pipingPosition[j]+1]);
				//isPiping = true;
			*/
				int command[2];
				pipe(command);
				pipeIn(command/* , command1, parameters[pipingPosition[j]-1]*/);
				pipeOut(command/*, command2, parameters[pipingPosition[j]+1]*/);
				close(command[0]);
				close(command[1]);
				close(0);
				close(1);
				printf("hi");				
execvp(argv[0], argv);
			}
			redirectionSearch = strchr(parameters[i],redirection);
			if (redirectionSearch!= NULL)
			{
				redirectionPosition[k] = i;
				printf("> in position %d\n", redirectionPosition[k]);
				isRedirecting = true;
				k++;
			}
			wildcardSearch = strchr(parameters[i],wildcard);
			if (wildcardSearch!= NULL)
			{	
				wildcardPosition[m] = i;
				printf("* in position %d\n", wildcardPosition[m]);
				isWildcard = true;
				m++;
			}

		}
		//Exiting out of the shell		
		int ret;
		ret = strcmp(parameters[0],"exit");
		if(ret == 0)
		exit(0);

		//Fork
		int rc = fork();
		//if Fork fails
		if(rc < 0)
		{
			printf("Oops! Something went wrong!");
			exit(0);
		}

		//Child Code
		else if(rc == 0) 
		{
			//printf("Child Runs\n");
			//This will be the final path to the program that we will pass to execv
                        char prog[512];
                        //First we copy a /bin/ to prog
                        strcpy(prog, path);
                        //Then we concancate the program name to /bin/
                        //If the program name is ls, then it'll be /bin/ls
                        strcat(prog, parameters[0]);
			if (isPiping == true)
			{
				//Piping code
			}
			if (isRedirecting == true)
			{
				//Redirection code
			}
			//Then execute the damn progra
			int retc = execve(prog,parameters,envList);
			if (retc == -1)
			{
				strcpy(prog,"");
				strcat(prog,parameters[0]);
				retc = execve(prog,parameters,envList);
				if(retc == -1)
				printf("The process %s could not be recognized.\n",c);
				return 0;
			}
		
			return 0;
		}

		//Parent Code
		else if(rc > 0)	
		{
			wait(NULL);	
		}
		
	}
}
