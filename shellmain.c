/*
CSCE 3600
Major 2
Group 10
Major 2
Members: 
Bibek Gyawali
Yen Pham
Geethareddy Kelam
Manasa Nimmagadda
*/


#include <stdio.h>
#include "headers.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>	

pid_t root_pid;				//to get the process id

int main(int argc, char *argv[])
{
root_pid = getpid();

/* Custom signal Handler */
signal(SIGINT, sigHandler);
signal(SIGTSTP, sigHandler);
char *buffer;
char **cmd;
char **arr;
char *test= malloc(sizeof(char)*100);

char* input = malloc(sizeof(char)*20);
char garb;
hvalue=0;

char* message = malloc(sizeof(char)*20);			//to display the message
char *test_buffer= malloc(sizeof(char)*512);			//to test
int exit_flag;
/* Interactive Mode */
initialize_history();
	if (argc==1)
		{
			/* Extra Credit Portion */
			printf("Do you want to customize your shell prompt:(Type y for yes) ");
			int retval1 = scanf("%[^\n]",input);
			if (retval1 == EOF) 
				{  /* Exit on Ctrl-D */
					printf("\nExiting with Ctrl + D in Interactive Mode\n");
					exit(0);
				}

			scanf("%c",&garb);

			if (strcmp(input,"y")==0 || strcmp(input,"Y")==0)
				{
					printf("Type your customzied shell prompt: ");
					int retval2 = scanf("%[^\n]",message);
					if (retval2 == EOF) 
					{  /* Exit on Ctrl-D */
						printf("\nExiting with Ctrl + D in Interactive Mode\n");
						exit(0);
					}

					scanf("%c",&garb);
				}
			else
				{
					message = "newshell";
				}
			// Asked user for command indefinately until user exits
			//allocating memory
			//cmd = malloc(sizeof(char*)*100);
			//buffer = malloc(sizeof(char)*512);

	/* Infinite loop till user tells us to stop */
		while (1)
		{	
			/* Memory allocating for buffer */
			cmd = malloc(sizeof(char*)*100);
			buffer = malloc(sizeof(char)*512);

			printf("%s: ",message);
			int retval = scanf("%[^\n]",buffer);

			if (retval == EOF) 
				{  /* Exit on Ctrl-D */
					printf("\nExiting with Ctrl + D in Interactive Mode\n");
					exit(0);
				}

			/* If a empty line */
			if (strlen(buffer)==0) {
				//	fflush(stdin);
					 scanf("%c",&garb);
					 continue;
					 }

			//if the line is more than 512 characters
			if (strlen(buffer)>512) {
					scanf("%c",&garb);
					continue;
					}

			set_history(buffer);
			int no_of_commands =0;
			exit_flag = 0;
			/* This loop is for the single line with multiple commands separated by to make an array of commands ; */
			while (1)
			{
				//for the first ;
				if (no_of_commands==0) { cmd[no_of_commands] = strtok(buffer,";"); }

				//rest of them 
				else {
					cmd[no_of_commands]=strtok(NULL,";");
				     }
				if (cmd[no_of_commands]==NULL) 
					{
					break;
					}
				no_of_commands++;
			}
			//printf("No of commands: %d",no_of_commands);
			//running all the commands
			for (int i=0; i<no_of_commands; i++)
			{

				if (cmd[i]==NULL) { continue; }
				if (strlen(cmd[i])==0) { continue; }
				memcpy(test_buffer,cmd[i],512);
				trimLeading(cmd[i]);
				if (strlen(cmd[i])==0) { continue; }
				test = strtok(test_buffer," ");

			/* Checks which command to execute */
			// If the user have typed exit somewhere in the line 
				if (strcmp(test,"exit")==0)
					{
						//printf("Set the flag to 1\n");
						exit_flag = 1;
					}
				else if (strcmp(cmd[i],"myhistory")==0)
					{
						history_cmd();
					}
				
				else if (strcmp(test,"path")==0 || strcmp(cmd[i],"echo $PATH")==0)
					{
						path_cmd(cmd[i]);
					}
				else if (strcmp(test,"cd")==0)		
					{
						processcdCommand(cmd[i]);
					}
				//custom shell scriptor
				else if (strcmp(test,"bash2")==0)
					{
						char* filename = malloc(sizeof(char)*512);
						filename = strtok(cmd[i]," ");
						filename = strtok(NULL, ";");
						initialize_batch_mode();
						custom_shell(filename);
					}
				//if i/o redirection
				else if (strchr(cmd[i],'<')!=NULL || strchr(cmd[i],'>')!=NULL)
					{
						//if pipe also present
						if (strchr(cmd[i],'|')!=NULL)
							{
								printf("%s: Pipe and I/O redirection cant be handled together\n",cmd[i]);
							}
						else {
						processRedirection(cmd[i]);
						}
					}
			//for all commands with or without buffer
				else
					{
						handle_command(cmd[i]);
					}
			}
			free(cmd);
			free(buffer);
			scanf("%c",&garb);
			//if the flag is set to 1
			if (exit_flag == 1) 
				{
						exit(0); 
				}
		}
	free(test_buffer);		
	}

/* Batch Mode */
	else if (argc==2)
		{
			//printf("Batch Mode\n");
			FILE * fp;
			fp = fopen(argv[1], "r");
			if (fp == NULL)
				{ 
					printf("cant open\n");
					return 0;
				}
			 char line [ 512 ];
			
			//reading until its end of file
     			 while (!feof(fp)) 
      				{
					//getting each line
					fgets (line, sizeof(line), fp );
					cmd = malloc(sizeof(char*)*100);	//memory allocation
			 		buffer = malloc(sizeof(char)*512);
					memcpy(buffer,line,512);
					//while reading from the file it takes the end of the line as one character
					//if the line is more than 512 characters
					if (strlen(buffer)<=1) { continue; }
					if (strlen(buffer)>512) { continue; }

					//fgets sometimes put \n on the line
			 		if (buffer[strlen(buffer)-1]=='\n') {		
					buffer[strlen(buffer)-1] = '\0';}

					set_history(buffer);
					memcpy(test_buffer,buffer,512);
					int no_of_commands = 0;
					printf("Executing command: %s\n",buffer);

					//parsing the line separating commands by ;
					while (1)
						{
							if (no_of_commands==0) { cmd[no_of_commands] = strtok(buffer,";"); }
							else {
								cmd[no_of_commands]= strtok(NULL, ";");
							     }
							if (cmd[no_of_commands]==NULL) 
								{
									break;
								}
							no_of_commands++;
						}
					exit_flag =0;
					for (int i=0; i<no_of_commands; i++)
						{
							//hit the end of the line
							if (cmd[i]==NULL) { continue; }
							if (strlen(cmd[i])==0) { continue; }
							trimLeading(cmd[i]);
							if (strlen(cmd[i])==0) { continue; }
							//set_history(cmd[i]);
							test = strtok(test_buffer," ");

							//checking to see which function to execute
							//when you perform exit, the flag is set to 1
							if (strcmp(cmd[i],"exit")==0)		
								{
									exit_flag = 1;
								}
							else if (strcmp(cmd[i],"myhistory")==0)
								{
									history_cmd();
								}
							else if (strcmp(test,"path")==0 || strcmp(cmd[i],"echo $PATH")==0)
								{
									path_cmd(cmd[i]);
								}
							else if (strcmp(test,"cd")==0)
								{
									processcdCommand(cmd[i]);
								}
							else if (strcmp(test,"bash2")==0)
								{
									char* filename = malloc(sizeof(char)*512);
									filename = strtok(cmd[i]," ");
									filename = strtok(NULL, ";");
									trimLeading(filename);
									//printf("Filename to be passed is: %s\n",filename);
									initialize_batch_mode();
									custom_shell(filename);
								}
							//for i/o redirection
							else if (strchr(cmd[i],'<')!=NULL || strchr(cmd[i],'>')!=NULL)
								{
									//if pipe also  present	
									if (strchr(cmd[i],'|')!=NULL)
										{
											printf("%s: Pipe and I/O redirection cant be handled together\n",cmd[i]);
										}
									else {
										processRedirection(cmd[i]);
										}

								}
							//for all regular commands with or without pipe
							else
								{
									handle_command(cmd[i]);
								}
						}
					//scanf("%c",&garb);
					free(cmd);
					if (exit_flag ==1) 
						{ 
						break; 
						}
				memset(buffer,'\0',512);
				memset(line, '\0',512);
				//memset(test_buffer, '\0',512);
				if(feof(fp)) { break; }	
				}
			free(test_buffer);
      			fclose ( fp );
		}
	else {
			printf("Improper number of arguments passed!\n");
		}
	
return 0;
}

//Signal handler
void sigHandler(int signum)
{
	pid_t my_pid = getpid();			//gets process id
	if(my_pid != root_pid)
	{
		//printf(" child process \n");
		signal(signum, SIG_DFL);		//reassigned to default
		kill (my_pid, signum); 			//kill the child process
		//exit(1);
	}
}