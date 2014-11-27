#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <fcntl.h>


//static char *line_read = (char *)NULL;

#define COLOR_RED	"\x1b[35m"
#define COLOR_BLUE	"\x1b[34m"
#define COLOR_RESET	"\x1b[0m" 

int main(int argc, char* argv[], char* envp[])
{

	printf(COLOR_BLUE"\n //----\\   ||       ||   ||  ||=====     //======  ||    ||  ||====  ||      ||\n");
	printf(" ||    ||  ||       ||   ||  ||          ||        ||    ||  ||      ||      ||\n");
	printf(" ||----    ||       ||   ||  ||=====     \\\\----\\\\  ||====||  ||====  ||      ||\n");
	printf(" ||    ||  ||       ||   ||  ||                ||  ||    ||  ||      ||      ||\n");
	printf(" ||____/   ||=====  \\\\---//  ||=====     ======//  ||    ||  ||====  ||====  ||====\n");
	printf(COLOR_RESET);

	char* c;
	char* parameters[128];
	char *cptr;
	int parameterCount = 0;
	
	//char piping = '|', redirection = '>', wildcard  ='*';

	//Little code for having the time displayed as well as the shell name
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	printf("\nLog-in time: %s",asctime(timeinfo));
	char* pegpoint = NULL;
	
        while(1)
	{
		int z;
 		char* path = "/bin/";
		char* envList[] = {"HOME=/root", "PATH=/bin:/sbin", NULL};
		bool chngdir = false;
		char *change = NULL;

		//Loop that resets arrays to null			
		for (z=0; z<= 128; z++)
		{
			parameters[z] = NULL;
		}
		parameterCount = 0;
		
		//Exiting out of the shell		
		int ret;
		ret = strcmp(parameters[0],"exit");
		if(ret == 0)
		{
			printf("\nLog-out time: %s\n",asctime(timeinfo));
			exit(0);
		}
	
		//Display and read the message
		int reg;
		//Current working directory display
		char cwd[1024];
		getcwd(cwd,sizeof(cwd));
		printf(COLOR_BLUE"\n(%s)"COLOR_RESET,cwd);
		
		if(pegpoint!=NULL)
		{
			printf(COLOR_RED"\t\t\t Peg:(%s)"COLOR_RESET,pegpoint);
		}

		//prompt display
		c = readline(COLOR_BLUE"\n[BLUE_SHELL  ]:- "COLOR_RESET);
		reg = strcmp(c,"");
		while (reg == 0)
		{	
			printf(COLOR_BLUE"\n(%s)\n"COLOR_RESET,cwd);
			c = readline(COLOR_BLUE"[BLUE_SHELL  ]:- "COLOR_RESET);
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

	
		//debugging.. set to 1 to see all the arguments on each seperate line
		if(0)
		{	
			int s = 0;
			while(s != parameterCount)
			{
				printf("%d: %s\n",s,parameters[s]);
				s++;
			}		
		}


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
			//This will be the final path to the program that we will pass to execv
                        char prog[1024];

                        //First we copy a /bin/ to prog
                        strcpy(prog, path);

			// Allowing for detection of piping, redirection
			int i;
			for (i = 0; i < parameterCount; i++)
			{	
				//Execute the program from bash, or by itself
				//Then we concancate the program name to /bin/
				//If the program name is ls, then it'll be /bin/ls
		                	
					//Special feature: Pegging
					int pegVal;
					pegVal = strcmp(parameters[0],"peg");
					if(pegVal == 0)
					{
						char dir[1024];
						pegpoint = getcwd(dir,sizeof(dir));
						continue;
					}
					pegVal = strcmp(parameters[0],"rmpeg");
					if(pegVal == 0)
					{
						if(pegpoint == NULL)
						{
							printf("No peg in place to remove. Type \"peg\" to place holder.\n");
							continue;
						}
						pegpoint = NULL;
						continue;
					}
					pegVal = strcmp(parameters[0],"mv2peg");
					if(pegVal ==0)
					{
						//printf("%s\n",pegpoint);
						if(pegpoint == NULL)
						{
							printf("No peg set in place to move to. Type \"peg\" to place holder.\n");
							continue;
						}
						int s;
						int counter = 0;
						int check;
						for(s=0;s<=strlen(pegpoint);s++)
							{
								printf("%c",pegpoint[s]);
								check = strncmp(&pegpoint[s],"/",1);
								if(check ==0)
									counter++; 	
							}
						printf("\nTotal /'s: %d\n",counter);
						char mover[128];
						strcat(mover,"../..");
						continue;
					}
					
					
					//Special Case for cd
					int cd = strcmp(parameters[i],"cd");
					if(cd == 0)
					{	
						change = parameters[1];
						cd = chdir(change);
						if(cd == 0)
							chngdir = true;
						else
							printf("%s:No such file or directory found!\n",parameters[1]); 	
						return 0;
					}
					
					//For other commands follow the path
					strcat(prog, parameters[i]);
					int retc = execve(prog,parameters,envList);
					if (retc == -1)
					{
						strcpy(prog,"");
						strcat(prog,parameters[i]);
						retc = execve(prog,parameters,envList);
						if (retc == -1)
						{
							strcpy(prog,"/usr/bin/");
							//printf("Checked /usr/bin\n");
							strcat(prog,parameters[i]);
							retc = execv(prog,parameters);
							if (retc == -1)
							{
								strcpy(prog,"/sbin/");
								strcat(prog,parameters[i]);
								retc = execv(prog,parameters);
								if(retc == -1)
								{
									printf("The process %s could not be recognized.\n",c);
								}
							 }
				         	}
				}		
			}
		}

		//Parent Code
		else if(rc > 0)	
		{
			wait(NULL);
			//printf("Parent finished waiting\n");
			change = parameters[1];
			if(chngdir == true);
				chdir(change);	
		}
			
	}
}



