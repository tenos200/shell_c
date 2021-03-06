#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include "alias.h"

void add_alias(char **tokens, int args) {

	int count = 0;

	char *str = tokens[1];
	char *str1;

	//if alias command entered is empty then print error
	if (tokens[2] == NULL) {
		printf("Not enough arguments given, try: alias <name> <command>\n");
		return;
	}
	str1 = concat_str(tokens, args);

	//checks the amount of elements in the list
	for (int i = 0; i < max_alias_size; i++) {
		if(strcmp(alias_map[i].aliasName, "") == 0) {
			count = i;
			break;
		}
	}

	//if alias is full then print message and return
	if(count >= max_alias_size) {
		printf("Maximum amount of alias has been set.\n");
		return;
	} else {

		//for loop for checking if the alias already exists
		for (int i = 0; i < max_alias_size; i++) {
			if(strcmp(alias_map[i].aliasName, str) == 0) {
				printf("alias %s %s, replaced with: %s %s\n", str, alias_map[i].aliasCommand, str, str1);
				strcpy(alias_map[i].aliasCommand, str1);
				return;
			}
		}

		//if alias does not exist, then add them to next available spot in array, i.e count
		strcpy(alias_map[count].aliasName, str);
		strcpy(alias_map[count].aliasCommand, str1);

	}

}


void remove_alias(char **tokens, int args) {
	
	char *command;
	int count = 0;
	int index;

	if(tokens[1] == NULL) {
		printf("Not enough arguments given, try: unalias <command>\n");
	}

	command = concat_str(tokens, args);

	//in order to count how many elements are in the array
	for(int i = 0; i < max_alias_size; i++) {
		if(strcmp(alias_map[i].aliasCommand, "") == 0) {
			count = i;
			break;
		}
	}

	//work in progress
	for(int i = 0; i < max_alias_size; i++) {
		if(strcmp(command, alias_map[i].aliasCommand) == 0) {
			strcpy(alias_map[i].aliasCommand, "");
			strcpy(alias_map[i].aliasCommand, "");

		}

	}


}

void invoke_alias() {


}

void print_alias() {

	for(int i = 0; i < max_alias_size; i++) {

		if(strcmp(alias_map[i].aliasName, "") == 0) {
			if(i == 0) {
				printf("No aliases have been added\n");
			}
			break;
		} 

		printf("%s = %s\n", alias_map[i].aliasName, alias_map[i].aliasCommand);

	}

}

char *concat_str(char **tokens, int args) {
	
	//allocates space to tokens array in order to allow mapping to stored properly
	char *store = malloc(sizeof(*tokens));

	//this for loop concatenates everything after str1 as one string for storing
	for(int i = 2; i < args; i++) {
		strcat(store, tokens[i]);
		strcat(store, " ");
	}

	//stores the concatenated string into arguments2
	char *str = store;

	//free allocated memory for store
	free(store);

	return str;
}
