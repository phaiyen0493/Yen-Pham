// Yen Pham


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>   
#include <string.h>
#include<sys/wait.h>
#include "headers.h"

pid_t pid, wpid;
int status;
int pipe1[2];
int pipe2[2];
int pipe3[2];

void exec(char* command);

void handle_command(char* cmd) 
{
	int command_count = 0;
	char *command[512];
	//seperate command based on the pipes
	command[command_count] = strtok(cmd, "|");
	command_count++;
	
	while(1)
	{
		command[command_count] = strtok(NULL, "|");
		if(command[command_count] == NULL) {break;}
		trimLeading(command[command_count]); //trim the beginning of the commands
		command_count++; //count the number of commands seperated by pipe(s)
	}
	command[command_count] = NULL;
	
	if (command_count == 1)
	{
		if ((pid = fork()) < 0)
		{
			perror("bad fork"); //display if fail to create children
			exit(1);
		}
		else if ( pid == 0 )	 // child
		{
			exec(command[0]); //execute the first command
			exit(0);
		}
		else	 // parent waits until the children finish and terminate
		{
			do 
			{
				wpid = waitpid(pid, &status, WUNTRACED);
    			} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		}
		
	}

	else if (command_count == 2)
	{
		// create pipe1	
		if (pipe(pipe1) == -1) 
		{
			perror("bad pipe1"); //display if fail to create pipe
			exit(1);
		}

		// fork command 1
		if ((pid = fork()) < -1)
		{
			perror("bad fork"); //display if fail to create children
			exit(1);
		}
		else if (pid == 0)
		{
			close(pipe1[0]); //close unused pipe
			dup2(pipe1[1], fileno(stdout) ); //instead of print out in file descriptor stdout, put the output in pipe
			close(pipe1[1]); //close when finish using

			//execute command 1
			exec(command[0]);
			exit(0);			
		}
		// parent
		else
		{
			do 
			{
				//wait until chidlren finish and terminate
				wpid = waitpid(pid, &status, WUNTRACED);
    			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			if ( WIFEXITED(status) )
			{
				pid = fork();
				if (pid < 0) 
				{
					perror("bad fork"); //display if fail to fork
					exit(1);
				}
				else if (pid == 0)
				{
					close(pipe1[1]); //close unsused pipe
					dup2(pipe1[0], fileno(stdin) ); //read the output of the first command into pipe1, instead of stdin
					close(pipe1[0]); //close when finish using 

					//execute command 2
					exec(command[1]);
					exit(0); //exit after done
				}
				else
				{
					close(pipe1[0]); //close unused pipe
					close(pipe1[1]); //close unused pipe

					//parent wait until the children terminate
					do 
					{
						wpid = waitpid(pid, &status, WUNTRACED);
    					} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				}
			}

			
		}
	}
	else	 // if (command_count >= 3)
	{
		// create pipe1
		if (pipe(pipe1) == -1) 
		{
			perror("bad pipe1");
			exit(1);
		}
		// create pipe2
		if (pipe(pipe2) == -1) 
		{
			perror("bad pipe1"); //display if fail to create pipe
			exit(1); //exit after done
		}

		// fork command 1
		if ( (pid = fork()) == -1 )
		{
			perror("bad fork1"); //display if fail to create children
			exit(1);
		}
		else if (pid == 0)
		{
			close(pipe1[0]); //close unused pipe
			dup2(pipe1[1], fileno(stdout)); //put output into one head of the pipe
			close(pipe1[1]); //close pipe after using
			close(pipe2[0]); //close unused pipe
			close(pipe2[1]); //close unused pipe

			//execute command 1
			exec(command[0]);
			exit(0); //exit after done
		}
		// parent
		else
		{
			//wait until children terminate			
			do 
			{
				wpid = waitpid(pid, &status, WUNTRACED);
    			} while (!WIFEXITED(status) && !WIFSIGNALED(status));

			if ( WIFEXITED(status) )
			{

				// fork for 2nd command
				if ((pid = fork()) == -1)
				{
					perror("bad fork2"); //display when fail to fork
					exit(1);
				} 
				else if (pid == 0)
				{
					close(pipe1[1]); //close unused pipe
					dup2(pipe1[0], fileno(stdin)); //read in the previous command output 
					close(pipe1[0]); //close pipe after using
	
					close(pipe2[0]); //close unused pipe
					dup2(pipe2[1], fileno(stdout)); //print out the output into the other head of the pipe
					close(pipe2[1]);//close pipe after using

					// exec command 2
					exec(command[1]);
					exit(0); //exit when done
				}
				// parent
				else
				{
					close(pipe1[0]); //close unused pipe
					close(pipe1[1]); //close unused pipe

					// wait until the children terminate
					do 
					{
						wpid = waitpid(pid, &status, WUNTRACED);
	    				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
					
					if ( WIFEXITED(status) )
					{

						// fork for 3rd command
						if ((pid = fork()) == -1)
						{
							perror("bad fork2"); //fail if fail to fork
							exit(1);
						} 
						else if (pid == 0)
						{
							close(pipe2[1]); //close unused pipe
							dup2(pipe2[0], fileno(stdin)); //read in the output of the previous command
							close(pipe2[0]); //close after using

							// exec command 3
							exec(command[2]);
							exit(0); //terminate after execution
						}
						else 	 // parent
						{
							close(pipe2[0]); //close unused pipe
							close(pipe2[1]); //close unused pipe	
				
							//wait until the children terminated
							do 
							{
								wpid = waitpid(pid, &status, WUNTRACED);
		    					} while (!WIFEXITED(status) && !WIFSIGNALED(status));
						}
					}
				}
			}
		}
	}
}

void exec(char* command)
{
	int index = 0;

	//seperate the command base on space
	char *arg[100];
	arg[0] = strtok(command, " ");
	index++;

	while (1)
	{
		arg[index] = strtok(NULL, " ");//break down the next arguments
		if(arg[index] == NULL)
		{
			break;
		}
		index++;
	}
	
	command = NULL; // clear the command

	//ignore the built in command
	if (strcmp(arg[0], "cd") == 0 || strcmp(arg[0], "history") == 0 || strcmp(arg[0], "exit") == 0 || strcmp(arg[0], "path") == 0)
	{
		printf("%s: Command not found\n", arg[0]); //print if command is built-in
	}

	//for testing signal only
	//sleep(5); 
	// exec
	execvp(arg[0], arg);
	printf("%s: Command not found\n", arg[0]); //input if execution fail
	//exit(0);
	//fflush(stdin);	
}