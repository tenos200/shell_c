#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include "alias.h"
#include "history.h"

static char *file;
static alias alias_map[max_alias_size];
static int alias_len = 10;

void add_alias(char **tokens, int args) {

	alias_len = max_alias_size;

	char *str = tokens[1];

	//if alias command entered is empty then print error
	if (tokens[2] == NULL) {
		printf("Not enough arguments given, try: alias <name> <command>\n");
		return;
	}
	//allocates space to tokens array in order to allow mapping to stored properly
	char *str1 = malloc(sizeof(char) * max_buffer_size);
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


	alias_len = number_of_aliases();

	//for loop for checking if the alias already exists
	for (int i = 0; i < alias_len; i++) {
		if(strcmp(alias_map[i].aliasName, str) == 0) {
			printf("alias %s %s, replaced with: %s %s\n", str, alias_map[i].aliasCommand, str, str1);
			strcpy(alias_map[i].aliasCommand, "");
			strcpy(alias_map[i].aliasCommand, str1);
			return;
		}

	}
	
	//if alias is full then print message and return
	if(alias_len >= max_alias_size - 1) {
		printf("Maximum amount of alias has been set.\n");
		return;
	}

	//if alias does not exist, then add them to next available spot in array, i.e count
	strcpy(alias_map[alias_len].aliasName, str);
	strcpy(alias_map[alias_len].aliasCommand, str1);

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
	
	count = number_of_aliases();

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
	if (index == -1 || count == 0) {
		printf("%s is not an alias.\n", command);
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

char *invoke_alias(char *fullinp, int invoke) {

	//used to check if there is any alias invoked, if check is 0 then we know that input is not alias
	int check = 0;
	//input to return and parse
	char inp[max_buffer_size];
	strcpy(inp, fullinp);
	strtok(inp, "\n");

	int size = number_of_aliases();

	char *alias_command = malloc(sizeof(char) * max_buffer_size);

	char *tok = strtok(inp, " ");
	//if alias is alias or unalias then return 0
	if(strcmp(tok, "alias") == 0 || strcmp(tok, "unalias") == 0) {
		return NULL;
	}

	while(tok != NULL) {

		for(int i = 0; i < size; i++) {
			//loop is used to concatenate the command that is being returned
			if(strcmp(tok, alias_map[i].aliasName) == 0) {
				tok = alias_map[i].aliasCommand;
				check++;
				break;
			}
		}
				
		strcat(alias_command, tok);
		strcat(alias_command, " ");
		tok = strtok(NULL, " ");

	}
	
	if(check == 0) {
		return NULL;
	} if(invoke == 0) {
		add_History(strtok(fullinp, "\n"));
	}

	return alias_command;
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

void load_alias() {

	char buffer[512];
	FILE *fp;
	
	//allocates memory to concat the home and history_file
	file = malloc(sizeof(char) * strlen(getenv("HOME")) + strlen(alias_file) + 1);
	//copys path to file
	strcpy(file, getenv("HOME"));
	//concatenates the file and history_file 
	strcat(file, alias_file);
	

	fp = fopen(file, "r");

	if(fp == NULL) {
		fp = fopen(file, "a");
	}


	//index for loop
	int index = 0;

	while(fgets(buffer, max_buffer_size, fp) != NULL) {

		//to check that file does not contain empty inputs
		if(strcmp(buffer,"\n") == 0) {
			printf("could not read .aliases\n");
			break;
		}

		//splits the string at the first space
		char *store = strtok(buffer, " ");
		strcpy(alias_map[index].aliasName, store);

		//takes the rest of the string after the first space to store as aliasCommand
		store = strtok(NULL, "\n");
		if(store == NULL) {
			printf("could not read .aliases\n");
			//clear the input that was previously added to the alias name since command in file is corrupt
			strcpy(alias_map[index].aliasName, "");
			break;
		} else {
			strcpy(alias_map[index].aliasCommand, store);
		}

		index++;
	}

	fclose(fp);
	free(file);

}

int number_of_aliases() {
	
	//checks the amount of elements in the list
	for (int i = 0; i < max_alias_size; i++) {
		if(strcmp(alias_map[i].aliasName, "") == 0) {
			alias_len = i;
			return alias_len;
		}
	}
	return max_alias_size;
}

void save_alias() {

	//allocates memory to concat the home and alias_file 
	file = malloc(sizeof(char) *strlen(getenv("HOME")) + strlen(alias_file) + 1);
	//copys path to file
	strcpy(file, getenv("HOME"));
	//concatenates the file and .aliases 
	strcat(file, alias_file);
	
	alias_len = number_of_aliases();

	FILE *fp = fopen(file,  "w");
	for (int i = 0; i < alias_len; i++) {
		fprintf(fp, "%s %s\n", alias_map[i].aliasName, alias_map[i].aliasCommand);
	}

	fclose(fp);
	free(file);

}
