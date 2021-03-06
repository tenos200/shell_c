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
int quit(char *path);

/*
The operation of the shell should be as follows:
Find the user homedir directory from the environment 
Set current working directory to user homedir directory 
Save the current path
Load history
Load aliases
Do while shell has not terminated
	Display prompt
	Read and parse user input
	While the command is a history invocation or alias, then replace it with the appropriate command from history or the aliased command respectively
	If command is built-in invoke, appropriate function
	Else execute command as an external process
End while
Save history
Save aliases
Restore original path Exit
*/

	

int main(void) {

	//gets the home directory
	char *path = getenv("PATH");
	char *homedir = getenv("HOME");
	char *username = getenv("USER");
	char inp[512];

	//sets starting directory as home
	chdir(homedir);
	//clear screen to look more like initalising shell
	system("clear");
	load_history();

	while(1) {
		printf("%s$ ", username);
		char *line = fgets(inp, 512, stdin);
		if(feof(stdin)) { //CTRL+D == EXIT
            return quit(path);
        } else {
			if(line[0] == '!') {
				//returns a string to parse
				line = invoke_History(line);
				//if return is NULL the history invokation was not valid 
				if(line != NULL) {
					parse_input(line , path, 1);
				}
			} else {
				parse_input(line, path, 0); // invoke = 0
			}
		}
		
	}
	//saves the history to file
	return 0;
}

// parse the user inputs and returns an array of tokens (t)

int parse_input(char *inp, char *path, int invoke){
	
	char fullinp[512];
	strcpy(fullinp, inp); // get full command line input
    char delim[] = " \t|<>&;\n";  
    
	char *token;
    char *tokens[50];                  //array of tokens (t)    
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
	} else if(strcmp(tokens[0],"setpath") == 0) {
		setPath(tokens, args);
	} else if(strcmp(tokens[0],"getpath") == 0) {
		getPath(args);
	} else if(strcmp(tokens[0], "alias") == 0) {
		 if(args == 1) {
			print_alias();
		} else {
			add_alias(tokens, args);
		}
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
