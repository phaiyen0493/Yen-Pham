#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <ctype.h>


//this file is for the built in command implementation

//cd - Geetha Kelam

void processcdCommand(char* cmd) 
{
	char * home_directory = malloc(sizeof(char)*512);
	home_directory = getenv("HOME");
	char* directory = malloc(sizeof(char)*512);
	directory = strtok(cmd," ");
	directory = strtok(NULL, " ");
	
	if (directory == NULL)
	{
		printf("%s\n", home_directory); 
	}
	else
	{
		if (chdir(directory) < 0)
		{
			perror("error\n");
		}
	}
}


//history //Done by Bibek

//initializing the history
void initialize_history()
	{
		//to store 20 commands
		history = malloc(sizeof(char*)*20);
		for (int i=0; i<20; i++)		//memory allocating for 20 spaces
			{
				history[i]=malloc(sizeof(char)*512);
			}

	}
void set_history(char* array)
        {
	//if value less than 20
           if (hvalue<20) 
            {
                memcpy(history[hvalue],array,512);
		//printf("Position %d set to %s\n",hvalue,array);
		hvalue++;
		
            }   
            else {
		//switching all commands space by one position
                for (int i=0; i<=18; i++)
                    {
			//printf("Swapped: %s & %s \n",history[i],history[i+1]);
                        memcpy(history[i],history[i+1],512);
			//printf("Swapped: %s & %s \n",history[i],history[i+1]);
                    }
		//store in the last one
                memcpy(history[19],array,512);
		//printf("Position %d set to %s\n",19,array);
            }
        }
//for printing it
void history_cmd()
	{
		int count;
		//printf("Entered into history_cmd mode\n");
		//if more than 20 commands print the latest 20 
		if (hvalue>19)			
			 {
				 count = 19;
			 }
		//else if whatever it is there
		else { count = hvalue-1; }
		for (int i=0; i<=count; i++)
			{
				printf("[%d]: %s\n",i+1, history[i]);
			}
		//for (int i=count; i>=0; i--)
			
	}

//triomming the spaces ahead and after any command
void trimLeading(char * cmd)
	{

	//start and stop variable
    	int start , st;
	
	//look from position 0
	start = 0;

	//look until the last position
    	 st = strlen(cmd) - 1;

	//isspace source library
	//checks any space present in the front of the string stops if sees a char other than space
    	while (1)	
		{
			if (!isspace((unsigned char) cmd[start]))
				{ break; }
			start++;
		}

	//it checks any spaces present in the end of the string
    	while (1)
        	{
			//decreasing the index every time
			if(st >= start && isspace((unsigned char) cmd[st]))
				{ st--; }
			else { break; } 
		}

	//shifting the char all the way
	int pos;
	for (pos = start ; pos <= st; pos++)
        	{
			cmd[pos - start] = cmd[pos];
		}

	//the last position of the string should always be null
    	cmd[pos - start] = '\0';
	}


//path
/* Yen's Part */
void delete_char(char *x,int a, int b) //delete characters 
{
	if ((a+b-1) <= strlen(x))
	{
		strcpy(&x[b-1],&x[a+b-1]);
	}
}

char* substr(const char *src, int m, int n) //get substring from index position m to index position n
{
	// get length of the destination string
	int len = n - m;

	// allocate (len + 1) chars for destination (+1 for extra null character)
	char *dest = (char*)malloc(sizeof(char) * (len + 1));

	// extracts characters between m'th and n'th index from source string
	// and copy them into the destination string
	for (int i = m; i < n && (*src != '\0'); i++)
	{
		*dest = *(src + i);
		dest++;
	}

	// null-terminate the destination string
	*dest = '\0';

	// return the destination string
	return dest - len;
}

char* remove_string(char * str, char * sub)
{
	char *p, *q, *r;
	//find the substring in the string
	if ((q = r = strstr(str, sub)) != NULL) 
	{
		//compare each character of the string
		size_t len = strlen(sub);
		while ((r = strstr(p = r + len, sub)) != NULL) 
		{
			while (p < r)
			{
  				*q++ = *p++;
			}
		}

		//terminate when the string reaches null
		while ((*q++ = *p++) != '\0')
		{
			continue;
		}
	}
	return str;
}

void path_cmd(char* cmd) //execute path command
{
	int index = 0;
	char *arg[512];
	char * temp_cmd = cmd;
	arg[index] = strtok(cmd, " ");
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

	// if the command has "path" or "echo $PATH" in the beginning
	if ((strcmp(arg[0], "path") == 0) || (strcmp(arg[0], "echo") == 0))
	{
		char *pathvar = getenv("PATH");

		// if the command is just path or echo $PATH
		if (index == 1 || strcmp(arg[0], "echo") == 0)
		{
			//print the current environment path
			printf("%s\n", pathvar);
		}
		else
		{
			//if command is path +, append " to both before and after the new path
			if (strcmp(arg[1], "+") == 0)
			{
				char temp[100];
				strcpy(temp, arg[2]);
				pathvar = strcat(pathvar, ":");
				pathvar = strcat(pathvar, temp);
				printf("%s\n", pathvar);
			}

			//if command is path -
			else if (strcmp(arg[1], "-") == 0)
			{
				char temp[100];
				char temp1[100];
				char temp2[100];
				strcpy(temp, ":");
				strcat(temp1, arg[2]); //temp1 = ./bin
				strcat(temp, temp1);
				strcpy(temp2, temp); // temp2 = :/bin
				strcat(temp, ":"); // temp = :./bin:

				// first look for :newpath:, if there is one then delete it in the string
				char *p = strstr(pathvar, temp);
				if (p)
				{
					pathvar = remove_string(pathvar, temp2);
					printf("%s\n", pathvar);
				}
				else //if the ending of the string contains the path to be deleted
				{
					char * last_directory = substr(pathvar, (strlen(pathvar) - strlen(temp2)), strlen(pathvar));
					if (strcmp(last_directory, temp2) == 0)
					{
						delete_char(pathvar, strlen(temp2) ,(strlen(pathvar) - strlen(temp2) + 1)); 
						printf("%s\n", pathvar);
					}
				}
			}
			else //print if the command does not exist
			{
				printf("Cannot execute this path command\n");
			}
		}
	}
}
