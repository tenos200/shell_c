#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include "alias.h"

void add_alias(char **tokens, int args) {

	int count = max_alias_size;

	char *str = tokens[1];

	//if alias command entered is empty then print error
	if (tokens[2] == NULL) {
		printf("Not enough arguments given, try: alias <name> <command>\n");
		return;
	}
	//allocates space to tokens array in order to allow mapping to stored properly
	char *str1 = malloc(sizeof(**tokens));
	//clears strings size malloc does not
	strcpy(str1, "");

	//this for loop concatenates everything after str1 as one string for storing
	for(int i = 2; i < args; i++) {
		strcat(str1, tokens[i]);
		//if there is nothing after it then dont add in space
		if(i == args -1) {
			break;
		}
		strcat(str1, " ");
	}

	//checks the amount of elements in the list
	for (int i = 0; i < max_alias_size; i++) {
		if(strcmp(alias_map[i].aliasName, "") == 0) {
			count = i;
			break;
		}
	}


	//for loop for checking if the alias already exists
	for (int i = 0; i < max_alias_size; i++) {
		if(strcmp(alias_map[i].aliasName, str) == 0) {
			printf("alias %s %s, replaced with: %s %s\n", str, alias_map[i].aliasCommand, str, str1);
			strcpy(alias_map[i].aliasCommand, "");
			strcpy(alias_map[i].aliasCommand, str1);
			return;
		}

	}

	//if alias is full then print message and return
	if(count >= max_alias_size) {
		printf("Maximum amount of alias has been set.\n");
		return;
	}

	//if alias does not exist, then add them to next available spot in array, i.e count
	strcpy(alias_map[count].aliasName, str);
	strcpy(alias_map[count].aliasCommand, str1);

}


void remove_alias(char **tokens, int args) {
	
	char *command;
	int count = max_alias_size;
	int index;

	if(tokens[1] == NULL) {
		printf("Not enough arguments given, try: unalias <command>\n");
		return;
	}
	
	
	//in order to count how many elements are in the array
	for(int i = 0; i < max_alias_size; i++) {
		if(strcmp(alias_map[i].aliasCommand, "") == 0) {
			count = i;
			break;
		}
	}

	if(count == 0) {
		printf("No aliases have been added\n");
		return;
	}

	//allocates space to tokens array in order to allow mapping to stored properly
	command = malloc(sizeof(*tokens));
	//clears strings size malloc does not
	strcpy(command, "");

	//this for loop concatenates everything after str1 as one string for storing
	for(int i = 1; i < args; i++) {
		strcat(command, tokens[i]);
		if(i == args -1) {
			break;
		}
		strcat(command, " ");
	}



	//removes the instance of the command that is in the alias_map
	for(int i = 0; i < max_alias_size; i++) {
		if(strcmp(command, alias_map[i].aliasCommand) == 0) {
			printf("Unaliased: %s\n", alias_map[i].aliasCommand);
			strcpy(alias_map[i].aliasCommand, "");
			strcpy(alias_map[i].aliasName, "");
			index = i;
			break;

		}
	}
	
	for(int i = index; i < max_alias_size - 1; i++) {
		//if the current spot is empty and the one infront is not them move it back
		if(strcmp(alias_map[i].aliasCommand, "") == 0 && strcmp(alias_map[i+1].aliasCommand, "") != 0) {
			//move statement infront to empty spot
			strcpy(alias_map[i].aliasCommand, alias_map[i+1].aliasCommand);
			strcpy(alias_map[i].aliasName, alias_map[i+1].aliasName);
			//make statement that has been moved empty
			strcpy(alias_map[i+1].aliasCommand, "");
			strcpy(alias_map[i+1].aliasName, "");
		}
	}

	//free memory that has been allocated
	free(command);

}

char *invoke_alias(char *fullinp) {

	//input to return and parse
	char inp[max_buffer_size];
	strcpy(inp, fullinp);
	strtok(inp, "\n");
	
	for(int i = 0; i < max_alias_size; i++) {
		if(strcmp(inp, alias_map[i].aliasName) == 0) {
			return alias_map[i].aliasCommand;
		}

	}

	return NULL;
}

void print_alias() {

	for(int i = 0; i < max_alias_size; i++) {

		if(strcmp(alias_map[i].aliasName, "") == 0) {
			if(i == 0) {
				printf("No aliases have been added\n");
			}
			break;
		} 

		printf("%s = '%s'\n", alias_map[i].aliasName, alias_map[i].aliasCommand);

	}
}
