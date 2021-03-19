#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include "history.h"
#include "alias.h"

int parse_input(char *inp, char *path, int invoke);
int process(char *tokens[50], int args);
void getPath(int args);
int changedir(char **tokens, int args);
void setPath(char **tokens, int args);
void commands(char **tokens, char *path, int args);
void checkAlias(char *str, char *line, char *path, int display);
int quit(char *path);

/*
 * Tim Enos
 * Adrian Pihlgren
 * Erin Allen
 * Karmen Tsang
 * Sudeep Dhakal
*/

	

int main(void) {

	//gets the home directory
	char *path = getenv("PATH");
	char *homedir = getenv("HOME");
	char *username = getenv("USER");
	char inp[max_buffer_size];
	//used to prevent multiple error messages begin displayed
	int display;
	//sets starting directory as home
	chdir(homedir);
	//clear screen to look more like initalising shell
	system("clear");
	load_history();
	load_alias();
	
	while(1) {
		printf("%s$ ", username);
		char *line = fgets(inp, max_buffer_size, stdin);
		display = 0;
		if(feof(stdin)) { //CTRL+D == EXIT
            return quit(path);
        } else if(invoke_alias(line, 1, display) != NULL) {
			//take out whatever is returned and store
			char *store = invoke_alias(line, 1, display);

			if(store[0] == '!') {
				//copy store over to line, to avoid segfault
				strcpy(line, store);
				//grab whatever is being invoked to parse as a command
				char *str = invoke_History(line);
				//pass to checkAlias function
				checkAlias(str, line, path, display);
			} else {
				//set display to 1 as we want to display any error messages
				display = 1;
				//take out whatever is returned and store
				store = invoke_alias(line, 0, display);
				//copy store over to line, to avoid segfault
				strcpy(line, store);
				//parse aliased input
				parse_input(line, path, 1);
			}
		} else if(line[0] == '!') {
				//returns a string to parse
				char *str = invoke_History(line);
				//pass to function to see if it is alias
				checkAlias(str, line, path, display);
		} else {
			parse_input(line, path, 0); // invoke = 0
		}
	}

	return 0;
}

int parse_input(char *inp, char *path, int invoke){
	
	int max_tokens_number = 50;
	char fullinp[max_buffer_size];
	char *token;
    char *tokens[max_tokens_number];                  //array of tokens (t)    

	strcpy(fullinp, inp); // get full command line input
    char delim[] = " \t|<>&;\n";  
    
	memset(tokens, 0, sizeof(tokens));
	int i = 0;
	token = strtok(inp, delim);   // delimiter to tokenize an array of strings

	while (token){
		tokens[i++] = token;
		token = strtok(NULL, delim);
	}
	
	// prevent null values being added to history
	if(*tokens == NULL) { 
		return 0;
	}
	
	//if not invoked then add to history
	if(invoke == 0) {
		add_History(strtok(fullinp, "\n"));
	} 

	commands(tokens, path, i); // execute command

	return 1;
}


void commands(char **tokens, char *path, int args) {

	if(strcmp(tokens[0], "exit") == 0) {
		quit(path);
	} else if (strcmp(tokens[0], "history") == 0) { // if user enters history
		if(args > 1)  {                        //error message if the user inputs history + something
            printf("Invalid number of arguments. Usage: history\n");
		} else {
			print_History(); 
        }
	} else if (strcmp(tokens[0], "cd") == 0)  { 
		if(args > 2) {                          // check for valid number of arguments
			printf("Invalid number of arguments. Usage: cd [path]\n");
		} else {
			changedir(tokens, args);
		}
	} else if(strcmp(tokens[0], "alias") == 0) {
		 if(args == 1) {
			print_alias();
		} else {
			add_alias(tokens, args);
		}
	} else if(strcmp(tokens[0], "unalias") == 0) {
		remove_alias(tokens, args);
	} else if(strcmp(tokens[0],"setpath") == 0) {
		setPath(tokens, args);
	} else if(strcmp(tokens[0],"getpath") == 0) {
		getPath(args);
	} else {
		process(tokens, args);
	}
}

// creating a separate process for fork() system call
int process(char *tokens[50], int args) {

	pid_t pid;
	pid = fork();                          // fork a child process
	
	if (pid < 0) {                         // error occurred
		fprintf(stderr, "Fork Failed!");   // prints error if fork failed
	} else if (pid == 0) {                 // if child process successful
		if (execvp(tokens[0], tokens) == -1) { 
			printf("shell: command not found %s\n", *tokens);
			
		}
		exit(0);
	} else {                               // parent process
		pid = wait(NULL);                  // wait for child process
	}
	   return 1;
}

void setPath(char **tokens, int args) {
    
    if (tokens[1] == NULL) {                    // print " if nothing is entered after setpath
        printf("No arguments found. Usage: setpath [dir]\n");
        return;
     }  else if (args > 2) {               // if more than one input print
        printf("Invalid number of arguments. Usage: setpath [dir]\n");
		return;
	}
    if (setenv("PATH", tokens[1], 1) == 0) {
        printf("PATH set successfully to %s\n", tokens[1]);
    } else {
        printf("Error setting path variable\n");
    }
}

void getPath(int args) {
	if (args == 1) { // if no arguments
		printf("PATH:%s\n", getenv("PATH"));
	} else { // if more than one argument, print appropriate error message
		printf("Invalid number of arguments. Usage: getpath \n");
	}
}

int quit(char *path) {

  if (setenv("PATH", path, 1) == 0) {
        printf("PATH restored to %s\n", getenv("PATH"));
    } else {
        printf("Error restoring path variable\n");
    }
    printf("Bye...\n");
	save_history();
	save_alias();
    exit(0);
}

//start on directory
int changedir(char **tokens, int args) {
	//only needs to redirect to home
	if(tokens[1] == NULL) {
		chdir(getenv("HOME"));
		return 1;
	} else if(chdir(tokens[1]) == -1) {
		perror("shell");
		return -1;
	}
	return 0;
}

void checkAlias(char *str, char *line, char *path, int display) {


	if(str != NULL) {
		//if str contains value then copy over to line
		strcpy(line, str);

		if(invoke_alias(line, 1, display) != NULL) {
			//set display to 1 as we want to display error messages
			display = 1;
			//take out whatever is returned and store
			char *store = invoke_alias(line, 1, display);
			//copy store over to line, to avoid segfault
			strcpy(line, store);
			//parse aliased input
			parse_input(line , path, 1);
		} else {
			strcpy(line, str);
			parse_input(line , path, 1);
		}
	}
}
