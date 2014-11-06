#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[])
{
	char c = '\0';
	printf("Welcome to the NeverWinter Shell!");
	printf("\n[MY_SHELL  ]:- ");
	while (c!= EOF)
	{
		c = getchar();
		//Store every word as an array of charachters to parse later

		//Once the words are stored, we can start the fork, exec and wait
		int rc = fork();

		if(rc < 0)
		{
			printf("Oops! Something went wrong!");
			exit(0);
		}

		else if(rc == 0)	// Now we can execute the command that we get from the array of charachters that we stored
		{
			printf("Executing command: ");
		}
		
		else if(rc > 0)		//Show wait() and then run MY_SHELL again
		{
			int wt = wait();
			printf("[MY_SHELL  ]:- ");
			break;
		}
	}
	printf("\n");
	return 0;
}
