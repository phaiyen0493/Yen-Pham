//Geethareddy Kelam

#include <stdio.h>
#include <stdlib.h>           
#include <ctype.h>            
#include <pthread.h>          
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/wait.h>

/**
 * Process the redirection
 */
void processRedirection(char* userInput) 
{
	//printf("Came into geetha's part\n");
        char delim[] = " ";
        // Split it and get command redirection symbol and file name
	char *args[28];
	char* filename;
	//getting tht filename
	filename = malloc(sizeof(char)*512);
	int index=0;
	int test;
	args[index] = strtok(userInput," ");
	while (1)
		{
			index++;
			args[index]=strtok(NULL, " ");
				if (strcmp(args[index],">")==0) 
				{
					args[index]=NULL;
					//printf("0\n");
					test=0;
					break;
				}
				if (strcmp(args[index],"<")==0)
				{
					args[index]=NULL;
					//printf("1\n");
					test =1;
					break;
				}
		}
	//for output redirection
       pid_t id = fork();
       if (id==0)
	{
		//if >
      		 if (test==0)
			{
				//reading filename
				filename = strtok(NULL, " ");
				//to pipe
                		int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                		if (fd < 0)
					{
                    				printf("Couldn't run\n");
						exit(0);
                			}
					//creating a pipe
                			dup2(fd, 1);
               				close(fd);
                			execvp(args[0], args); 
					printf("%s: command not found\n",args[0]);
					exit(0);					
            		}
		//if <
		else	
			{
				filename = strtok(NULL, " ");
				//printf("Filename for input redirection %s: \n",filename);
				int fd2 = open(filename, O_RDONLY);
                		//bad pipe
				if (fd2 < 0)
					{
                    				printf("Couldn't run\n");
						exit(0);
					}

					dup2(fd2,0);
					close(fd2);
					execvp(args[0], args);
					printf("%s: command not found\n",args[0]);
					exit(0);
			} 

		free(filename);
	}
	//parent wait for the child
	else if (id>0)
		{
			wait(NULL);
		}
	else
		{

			perror("fork errror\n");
		}
}



