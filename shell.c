#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[])
{
	char c[256];
	char* parameters[10];
	char *cptr;
	const char delimiters[2] = " ";
	printf("Welcome to the NeverWinter Shell!");
        while(1)
	{
		
		printf("\n[MY_SHELL  ]:- ");
		int l;
        	for (l = 0; l <= 256; l++)
		{
			c[l] = getchar();
			if(c[l] =='\n')
			break;
		}
	


        	//Store every word as an array of charachters to parse later
		cptr = strtok(c, " ");
		int count = 0;
		while (cptr != NULL)
		{
			//cptr = strtok(NULL, " ");
			parameters[count] = cptr;
			count++;
			cptr = strtok(NULL, " ");
		}
		printf("Amount of parameters: %d\n", count);
		int x = 0;
		for (x = 0; x <= count; x++)
		{
			printf("This is string %d:", x);
			printf(parameters[x]);
			printf("\n");
		}

		//printf(cptr);	
	}
		/*
		//Once the words are stored, we can start the fork, exec and wait
		int rc = fork();

		if(rc < 0)
		{
			printf("Oops! Something went wrong!");
			exit(0);
		}

		else if(rc == 0)	// Now we can execute the command that we get from the array of charachters that we stored
		{
			printf("Executing command: child ");
		}
		
		else if(rc > 0)		//Show wait() and then run MY_SHELL again
		{
			int wt = wait();
			printf("[MY_SHELL  ]:- ");
			break;
		}
	printf("\n");
*/
	return 0;
}
