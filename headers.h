//This is where we declare our functions prototype used in all the project



/* Used by Bibek */
char** history;
void initialize_history();
void trimLeading(char * str);
int hvalue;
void set_history(char* array);
void history_cmd();
void custom_shell(char* filename);
char* trim_string(char* name);
char ** string_variables;
char ** int_variables;
char ** string_values;
int values[50];
int string_count;
int int_count;
void initialize_batch_mode();
void handle_operator(char*);
int getvalue_of_int(char*);
void print_variable(char*);
void test_int();
/* Used by Bibek */


/* Geetha's Part */
void processcdCommand(char* cmd);
void processRedirection();

/* Yen Pham */
void path_cmd(char* cmd);
void handle_command(char* cmd);

/*Manasa*/
void sigHandler(int signum);
