GROUP 10 MAJOR 2

GROUP MEMBERS
1. Bibek Gyawali
2. Yen Pham

Compiling instructions

1. 
	Compile using -----> gcc *.c
 
	FOR INTERACTIVE MODE 
	----------> ./a.out (without any arguments)
	----------> type in commands when prompted

	FOR BATCH MODE
	-----------> ./a.out [batch-file]

2.
	Using make file
	Step 1: To make the file including dependencies: make                   
	Step 2: To run after make command: ./major OR ./major [BATCHFILE]                             
	Step 3: To clean the temporary object files and make function: make clean                   


Note: We have done the extra credit portion so; while on interactive mode, it will ask if you wanna customize the shell prompt or not.
      If you typed y or Y; you can type your message for the shell prompt.


Organization of the Project:

1. Bibek Gyawali
	: Wrote the main menu for the interactive and batch mode
	: Handled the myhistory builtin command
	: Build the custom bash scriptor
	: Made the make and readme file
	: handled the exit command incorporated in the main loop
	: handled the I/O redirection portion of executing the command

2. Yen Pham
	: handled the path builtin command
	: handled the exection of commands, also the ones with the pipes
	: handle signal's portion
	: handled the cd builtin command

DESIGN OVERVIEW

In main menu the program is divided into two parts, based on the arguments provided; with one arguments and without any arguments while running the program
While on one argument; batch mode is started and on zero argument interactive mode is started

PROGRAM OVERFLOW

INTERACTIVE MODE
	User is given option to select if they want to customize their shell prompt or no. Andthen lines of command are read, until users decides to quit
	Every line of command is parsed using strtok, ";" strtok as a deliminator and for every line an array of commands separated by ";" are obtained.
	Then every command is prepended to history before executing and the commands are compared if it contains the builtin commands like cd, path, exit.
	Every command is printed first before executing.
	signal handlers are properly initialized to handle the signals.
	Empty commands in between semicolons wont be executed as it wont fall under any category.
	If cd is present, the string after cd is obtained and passed to the processcdCommand function to handle it.
	If exit is present, the exit_flag is set to 1 and all the commands present in that line is run; and when it goes to read next line; it would exit the loop if the exit flag is 1.
	If myhistory is the command we parsed; a function to show history is called
	If path is present as the command, the whole command is passed to handle path function where it is added or subtracted.
	If I/O redirections symbols are present in the command; it is passed to processiodirection() function to properly handle and execute it.
	And the rest of the case for the command, it is passed to the command handler function which exectutes the commands in addition to handling any pipes present.
	If pipes and I/O redirection are present in the command; it is just ignored because it is not a functionality asked by the program.
	Empty lines are just ignored and wont be interpreted.
	Custom shell script interpretrer is invoked when seen a bash2 <filename> command where it would pass the file name to the custom_shell function to handle it. 


BATCH MODE
	The command line argument for the filename is taken and that file is opened in read mode and every line is read until the end of the file.
	No extra credit portion for customizing shell prompt here; we are just reading from file.
	The parsing the lines and running are exactly similar to the interactive mode which is;

	signal handlers are properly initialized to handle the signals.
 	Every line of command is parsed using strtok, ";" strtok as a deliminator and for every line an array of commands separated by ";" are obtained.
	Then every command is prepended to history before executing and the commands are compared if it contains the builtin commands like cd, path, exit.
	Every command is printed first before executing.
	If cd is present, the string after cd is obtained and passed to the processcdCommand function to handle it.
	If exit is present, the exit_flag is set to 1 and all the commands present in that line is run; and when it goes to read next line; it would exit the loop if the exit flag is 1.
	If myhistory is the command we parsed; a function to show history is called
	If path is present as the command, the whole command is passed to handle path function where it is added or subtracted.
	If I/O redirections symbols are present in the command; it is passed to processiodirection() function to properly handle and execute it.
	If pipes and I/O redirection are present in the command; it is just ignored coz it is not a functionality asked by the program.
	And the rest of the case for the command, it is passed to the command handler function which exectutes the commands in addition to handling any pipes present.
	Custom shell script interpretrer is invoked when seen a bash2 <filename> command where it would pass the file name to the custom_shell function to handle it.

BRIEF ON HOW EACH PART WORKS

Built in Command parts

1. cd
	----->
	----->
	----->

2. path
	-----> An array of string is created to hold each word seperated by space
	-----> If the string only have one word "path", call the string path by using getenv("PATH")
	-----> If the string have '+' sign after path, append : and the new path to the current path, by using strcat
        -----> If the string have '-' sign after path, first put : before and after the new path, then look for the string and delete that path string
	-----> If no :pathttofind: is found, then look for the end of the current path by using substring; if the end of the path has the substring similar to the string we need to delete, then delete that string

3. myhistory
	-----> An array of string is created to hold the latest 20 commands run in our custom shell
	-----> I have a global variable for the counter to count the number of commands
	-----> When the command I have is less than 20; when prepending the command, i just add it to the end empty slot on the array using a function
	-----> When there are already 20 slots fulfilled, i then add 21st slot replacing the first slot; moving all the commands already in the array by one position
	-----> When user types myhistory commands; i looped through the number of commands i have in the data structure(maximum of 20) and print it out to the user.

4. exit
	-----> This is not done in separate function, rather mixed in main file
	-----> Whila parsing if a exit is seen, the exit flag is set to 1 and that set exit flag will break out the loop while going to read next line while exiting.

Header Files
	-----> All the prototypes are present for the function
	-----> Several arrays af string and integers are declared to be used in the custom shell interpretor
	-----> Sevral intialize function are declared with intial values to help keep track of the builtin commands

Shell Functionality

1. I/O Redirection
	----->
	----->
	----->
	----->
	----->
	----->

2. Signaling
	----->
	----->
	----->
	----->
	----->
	----->

3. Piping and executing the command
	-----> An array of string is created to hold all the seperate command seperated by |
	-----> Fork children, in all the children branch, create the pipe(s), call the exec() function, then teminate when children process is finished
        -----> If command count is one, we don't create any pipe, when command count is 2, we create 1 pipe, and 2 pipes are created when command counts are 3
	-----> When exec function is called, an array of string is created to hold all the seperate words in the command, seperated by space " "; then execvp() buil-in function is called 
	-----> When creating the pipes, we use dup2 to read in from descriptor stdin and write from descriptor stdout
	-----> Parent will keep waiting until children's process is finished then come back to the main shell

4. Custom shell interpreter
	-----> A filename is read and the file is opened in read mode and every line is read until the end of file.
	-----> An array of string are declared to hold the string variables and another array of string are declared to hold integer variables in the header files.
	-----> An array of string is declared to store the string variables value and array of int is declared to store int variable value.
	-----> A line is checked for comment having "#" ; if exist in first positon all the line is ignored
	-----> A '=' is checked and the variable is stored on the basis of the value it contains; checked using "" for string values and another for int values
	-----> An operator '+' , '-' , '*' is handled by a functions(including all the helper functions) where the variable before = is appended
	-----> Also, the variables which are around the operator are passed to a function and its value is obtained to calculate.
	-----> Warning is displayed for variable not initialized, and a string variable is used while doing operator's stuff.
	-----> A display whenever seen is treated into two different parts the one with $ sign and one with "".
	-----> for the one with $ sign, it just goes to find the value of that variable and prints(if exits). If doesnt; variable not initialized printed to user.
	-----> For "" all the string inside is printed, just for one exception , if a $ sign is present it would treat it as variable and its value is printed(if exists)

KNOWN BUGS AND PROBLEMS
	------> Shell might not handle when a pipe and I/O redirections are passed in a single line
	
 
	
