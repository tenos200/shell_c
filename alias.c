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

	free(str1);

}


void remove_alias(char **tokens, int args) {
	
	char *command = tokens[1];
	int count = max_alias_size;
	//if the index is not changed then alias does not exist to remove
	int index = -1;

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

	//checks if the alias exists if so then remove it.
	for(int i = 0; i < count; i++) {
		if(strcmp(command, alias_map[i].aliasName) == 0) {
			printf("Unaliased: %s\n", alias_map[i].aliasName);
			strcpy(alias_map[i].aliasName, "");
			strcpy(alias_map[i].aliasCommand, "");
			//set index to position that was replaced
			index = i;
		}
	}

	//if the index is not changed then alias does not exist to remove
	if (index == -1) {
		printf("%s can not be unaliased because it is not an alias.\n", command);
		return;
	}

	for(int i = index; i < max_alias_size - 1; i++) {
		//if the current spot is empty and the one infront is not them move it back
		if(strcmp(alias_map[i].aliasName, "") == 0 && strcmp(alias_map[i+1].aliasName, "") != 0) {
			//move statement infront to empty spot
			strcpy(alias_map[i].aliasName, alias_map[i+1].aliasName);
			strcpy(alias_map[i].aliasCommand, alias_map[i+1].aliasCommand);
			//make statement that has been moved empty
			strcpy(alias_map[i+1].aliasCommand, "");
			strcpy(alias_map[i+1].aliasName, "");
		}
	}

}

char *invoke_alias(char *fullinp) {

	//input to return and parse
	char inp[max_buffer_size];
	strcpy(inp, fullinp);
	strtok(inp, "\n");
	
	for(int i = 0; i < max_alias_size; i++) {
		if(strcmp(inp, alias_map[i].aliasName) == 0) {
	
			char *store = malloc(sizeof(alias_map[i].aliasCommand));
			strcpy(store, alias_map[i].aliasCommand);
			return store;
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
