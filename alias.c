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

	char *args1 = tokens[1];
	char *args2 = tokens[2];
	//allocates space to tokens array in order to allow mapping to stored properly
	char *store = malloc(sizeof(*tokens));

	//if alias command entered is empty then print error
	if (args2 == NULL) {
		printf("Not enough arguments given, try: alias <name> <command>\n");
		return;
	}

	//this for loop concatenates everything after args2 as one string for storing
	for(int i = 2; i < args; i++) {
		strcat(store, tokens[i]);
		strcat(store, " ");
	}

	//stores the concatenated string into arguments2
	char *arguments2 = store;

	//free allocated memory for store
	free(store);

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
			if(strcmp(alias_map[i].aliasName, args1) == 0) {
				printf("alias %s %s, replaced with: %s %s\n", args1, alias_map[i].aliasCommand, args1, arguments2);
				strcpy(alias_map[i].aliasCommand, arguments2);
				return;
			}
		}

		//if alias does not exist, then add them to next available spot in array, i.e count
		strcpy(alias_map[count].aliasName, args1);
		strcpy(alias_map[count].aliasCommand, arguments2);

	}

}


void remove_alias() {


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
