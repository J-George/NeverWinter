
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

static char *line_read = (char *)NULL;

#define COLOR_BLUE	"\x1b[34m"
#define COLOR_RESET	"\x1b[0m" 


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
	int  pipingPosition[64], redirectionPosition[64], wildcardPosition[64];


	char* c;
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
		for (z=0; z<= sizeof(c); z++)
		{
			c[z] = '\0';
			parameters[z] = NULL;
		}
		parameterCount = 0;
		
		//Display and read the message
		int reg;
		c = readline(COLOR_BLUE"[BLUE_SHELL  ]:- "COLOR_RESET);
		reg = strcmp(c,"");
		while (reg == 0)
		{
			c = readline(COLOR_BLUE"[BULE_SHELL  ]:- "COLOR_RESET);
			reg = strcmp(c,"");
			if (reg != 0)
			break;
		}
		add_history(c);

        	//Store every word as an array of strings to parse later
		cptr = strtok(c, " ");
		while (cptr != NULL)
		{
			parameters[parameterCount] = cptr;
			parameterCount++;
			cptr = strtok(NULL, " ");	
	 	}

		// Allowing for detection of piping, redirection and wildcard implementation
		int i, j = 0, k = 0, m = 0;
		for (i = 0; i < parameterCount; i++)
		{
			pipeSearch = strchr(parameters[i],piping);
			if (pipeSearch!= NULL)
			{
				pipingPosition[j] = i;
				printf("| in position %d\n", pipingPosition[j]);
				isPiping = true;
				j++;
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

		//debugging.. set to 1 to see all the arguments on each seperate line
		if(0)
		{	
			int s = 0;
			while(s != parameterCount)
			{
				printf("%s\n",parameters[s]);
				s++;
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
                        char prog[1024];
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
				retc = execv(prog,parameters);
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
