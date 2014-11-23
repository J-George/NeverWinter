#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[])
{
	char c[256] = "\0";
	char* parameters[256];
	char *cptr;
	int parameterCount = 0;
	printf("Welcome to the NeverWinter Shell!");
        while(1)
	{
		int z;

		//Loop that resets arrays to null
		for (z=0; z<=256; z++)
		{
			c[z] = '\0';
			parameters[z] = '\0';
		}
		parameterCount = 0;
		printf("\n[MY_SHELL  ]:- ");
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

		//Fork
		int rc = fork();
		//if Fork fails
		if(rc < 0)
		{
			printf("Oops! Something went wrong!");
			exit(0);
		}

		else if(rc == 0) //Code for child to run
		{
			printf("Child Runs\n");
			return(0);
		}
		else if(rc > 0)	//Parent waits for child to finish running then restars while loop
		{
			printf("Parent starts waiting\n");
			int wt = wait();
			printf("Parent is finished waiting\n");	
		}
		printf("\n");
	}
}
