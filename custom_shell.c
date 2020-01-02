/* Bibek Gyawali
Group 10
Major 2
*/
#include <stdio.h>
#include "headers.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


//initializing all the variables used and allocating memory for it
void initialize_batch_mode()
	{
		string_variables = malloc(sizeof(char*)*50);
		int_variables = malloc(sizeof(char*)*50);

		//char **array = malloc(totalstrings * sizeof(char *));
		string_values = malloc(sizeof(char*)*50);
		string_count=0;
		int_count=0;
	}

//trims the string the first character
char* trim_string(char* name)
	{	
        for (int i=0; i<strlen(name)-1; i++)
        	{
            name[i]= name[i+1];
        	}
    	name[strlen(name)-1]= '\0';
    	return name;
    	}

//checks if the string is just made up of digits or not
bool isnumber(char* c)	
	{
		for (int i=0; i<strlen(c); i++)
			{
				if (!isdigit(c[i])) { return false; }
			}
		
		return true;
	}
//checks if the variable is int_variable or not
int is_int_variable(char* c)
	{
		//looping through all the integer variables & searching
		for (int i=0; i<int_count; i++)
			{
				if (strcmp(c,int_variables[i])==0)
					{
						return true;
					} 
			}
		//looping through all the string variables & searching to give error to user
		for (int i=0; i<string_count; i++)
			{
				 if (strcmp(c,string_variables[i])==0)
					{
						printf("%s is a string variable\n",c);
						return false;
					} 
			}
	//if no where found
	printf("%s not initialized!\n",c);
	return false;
	
	}

//gets the value of integer variable
int getvalue_of_int(char* c)
	{
		if (isnumber(c)) { return atoi(c); }
		int value=0;
		for (int i=0; i<int_count; i++)
			{
				if (strcmp(c,int_variables[i])==0)
					{
						value = values[i];
					} 
			}
	      return value;

	}
//removes the first character in the word used to trim ' " '
void remove_first(char* word)
	{
		for (int i=0; i<strlen(word); i++)
			{
				word[i]=word[i+1];
			}
	}

//handles addition, subtraction and multiplication
void handle_operator(char* line)
	{
		//memory allocation for new variable
		char* new_var;
		char* chd;
		new_var = malloc(sizeof(char)*512);
		chd = malloc(sizeof(char)*512);
		//printf("Handling: %s\n",line);
		if (strchr(line, '+')!=NULL)						//if addition
			{
				new_var = strtok(line,"=");				//getting the new variable
				trimLeading(new_var);
				int_variables[int_count]=malloc(sizeof(char)*512);
				memcpy(int_variables[int_count],new_var,512);
				chd = strtok(NULL,"+");					//first variable to add
				trimLeading(chd);
				if (chd[0]=='$') { remove_first(chd); }
				if (!isnumber(chd) && !is_int_variable(chd))		//checking if valid variable or not
						{
							return;
						}	
				int total = getvalue_of_int(chd);			//getting the value of the variable
				chd = strtok(NULL,"\n");				//getting the second aka last variable
				trimLeading(chd);
				if (chd[0]=='$') { remove_first(chd); }
						if (!isnumber(chd) && !is_int_variable(chd))	//valid or not
						{
							return;
						}				
				total = total + getvalue_of_int(chd);			//gettting the variable value	
				values[int_count]=total;
				int_count++;						//increasing the count of integer variable
			}	
		else if (strchr(line, '-')!=NULL)					//for subtraction everything same as addition
			{
				new_var = strtok(line,"=");
				trimLeading(new_var);
				int_variables[int_count]=malloc(sizeof(char)*10);
				memcpy(int_variables[int_count],new_var,10);
				chd = strtok(NULL,"-");
				trimLeading(chd);
				if (chd[0]=='$') { remove_first(chd); }
					if (!isnumber(chd) && !is_int_variable(chd))
						{
							return;
						}
				int total = getvalue_of_int(chd);
				chd = strtok(NULL,"\n");
				trimLeading(chd);
				if (chd[0]=='$') { remove_first(chd); }
					if (!isnumber(chd) && !is_int_variable(chd))
						{
							return;
						}
				total = total - getvalue_of_int(chd);
				values[int_count]=total;
				int_count++;
			}
		else 										//for multiplication; everything same as addition
			{
				new_var = strtok(line,"=");
				trimLeading(new_var);
				int_variables[int_count]=malloc(sizeof(char)*10);
				memcpy(int_variables[int_count],new_var,10);
				chd = strtok(NULL,"*");
				trimLeading(chd);
				if (chd[0]=='$') { remove_first(chd); }
					    if (!isnumber(chd) && !is_int_variable(chd))
						{
							return;
						}
				int total = getvalue_of_int(chd);
				chd = strtok(NULL,"\n");
				trimLeading(chd);
				if (chd[0]=='$') { remove_first(chd); }
						if (!isnumber(chd) && !is_int_variable(chd))
						{
							return;
						}
				total = total * getvalue_of_int(chd);
				values[int_count]=total;
				int_count++;
			}
	//free(new_var);
	//free(chd);
	}

//function to print the variable value
void print_variable(char* c)
	{	
		bool test = false;
		int value;
		int loc;
		//if found on string 
		for (int i=0; i<string_count; i++)
			{
				if (strcmp(string_variables[i],c)==0)
					{	
						loc = i;
						test = true;						
					}
			}
		//print string variable value
		if (test==true) { 
					printf("%s\n",string_values[loc]);
					return;
				}

		//if found on integer
		for (int i=0; i<int_count; i++)
			{
				if (strcmp(int_variables[i],c)==0)
					{
						test = true;
						value = values[i];
					}

			}
		//print int variable value
		if (test==true) 
			{ 
				printf("%d\n",value);
				return;
			}
		//if nowhere found
		printf("%s variable not declared\n",c);
	}


//checks if the line is comment or not; the first character in that line should be #
bool is_a_comment(char* line)
	{
	for (int i=0; i<strlen(line); i++)
		{
			//if space check next character
			if (line[i]==' ')
				{	
					continue;	
				}
			//if yes
			if (line[i]=='#')
				{
					return true;
				}
			return false;
		}

	}

//this is for the handling of $ variable inside the inverted comma while displaying the string 
void handle_special_display(char* word)
	{
		//printf("came to special case:\n");
		int strlen_count = 0;
		for (int i=0; i<strlen(word); i++)
			{
			//checking how many space ahead is that variable which should be printed
			if (word[i]==' ') { strlen_count++; }
			if (word[i]=='$')
				{
					//trimming that variable and getting its value to get printed using strtok
					char * str = malloc(sizeof(char)*10);
					str = strtok(word, " ");
					//printf("str: %s",str);
					for (int j=0; j<strlen_count; j++)
						{
							str = strtok(NULL, " ");
						}
					remove_first(str);
					print_variable(str);
				}

			//until variable found keep on printing the characters whatever it is on inside the inverted comma
			else
				{
					printf("%c",word[i]);
				}
			}
		//printf("\n");
	}

//main shell function
void custom_shell(char* filename)
	{
	//printf("You are on custom shell display module\n");
	//to store each line
	char line [512];
	char buffer [512];
	//array of commands
	char ** cmd;
	cmd = malloc(sizeof(char*)*2);
	//to open file
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL)								//errror
		{
			printf("Cant open the file for custom shell input\n");
			return;
		}
	//parsing each line till eof
	while ( fgets (line, sizeof(line), fp)!=NULL )
		{
			bool test=true;
			memcpy(buffer,line,512);
			cmd[0] = malloc(sizeof(char)*512);
			cmd[1] = malloc(sizeof(char)*512);
			//checking for comment
			if (is_a_comment(line))
				{
					continue;
				}
			//checking for assignment operator
			 if (strchr(line, '=')!=NULL && strstr(line,"display")==NULL)
				{
					cmd[0] = strtok(buffer, " ");
					if (strcmp(cmd[0],"display")==0) { break; }

					//operator handling
			 		if (strchr(line, '+')!=NULL || strchr(line, '-')!=NULL || strchr(line,'*')!=NULL)
						{
							handle_operator(line);
						}

					//variable assignment
					else
						{
							char* ch;
							ch = malloc(sizeof(char)*512);
							char* tch;
							tch = malloc(sizeof(char)*512);
							tch = strtok(line,"=");						//getting the variable name
							trimLeading(tch);
							ch = strtok(NULL, "\n");
							trimLeading(ch);
							if (ch[0]=='"')							//if string
								{	
									ch = trim_string(ch);				//trimming for the spaces and all
									int l = strlen(ch);
									ch[l-1] = '\0';
									//allocating memory for the variable to be stored
									string_variables[string_count]=malloc(sizeof(char)*10);
									string_values[string_count]=malloc(sizeof(char)*512);
									//storing the string variables names and value in our array
									memcpy(string_variables[string_count],tch,10);
									memcpy(string_values[string_count],ch,512);
									string_count++;					//increasing the variale count				
								}
							else								//if integer
								{
									//storing the int variables names and values in our array
									int_variables[int_count]=malloc(sizeof(char)*512);
									memcpy(int_variables[int_count],tch,10);
									values[int_count]=atoi(ch);
									int_count++;					//increasing the variable count
								}	
							
						}
			
				}
			 cmd[0] = strtok(line," ");
			 if (strcmp(cmd[0],"display")==0) 								//if display 
				{
					cmd[1]= strtok(NULL,"\n");
					//printf("Display: %s\n",cmd[1]);
					if (cmd[1][0]=='$')								//displaying a variable
							{
								cmd[1]= trim_string(cmd[1]);
								print_variable(cmd[1]);
							}
					else
							{
								cmd[1] = trim_string(cmd[1]);
								int l = strlen(cmd[1]);
								cmd[1][l-1] = '\0';				//trimmed the last inverted comma
								//printf("Display: %s\n",cmd[1]);
								if (strchr(cmd[1],'$')!=NULL)			//displaying the special case; variable inside the inverted comma
									{
										handle_special_display(cmd[1]);
									}
								else { printf("%s\n",cmd[1]); }
							}
				}
			
		}
		//freeing our pointers
		free (int_variables);
		free (cmd);
		free (string_variables);
	
	}
