#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include "history.h"

//used to keep track of history count
static int history_counter = 0;
//array for storing history
static char history[20][512];
//char array for storing path to .hist_list
static char *file;
		
char *invoke_History(char *inp) {
	
	int i, trailing = -1;
	int index;
	//use sscanf to take out index and trailing garbage for comparison
	int comp = sscanf(inp, "!%d%n", &index, &trailing);

	//check if last command is invoked
	if(strcmp(inp, "!!\n") == 0) {

		//if counter is 0 then no command can be invoked print error
		if(history_counter == 0) {
			printf("Error: No history to invoke.\n");
			return NULL;
		} 
		//otherwise execute the previous command
		else {
			return history[history_counter-1];
		}
	} 
	//checks whether comp is vaild and if trailing length is equal to length of input, if so then command is valid.
	else if(comp == 1 && trailing == strlen(inp) - 1) {
		//checks if the index is valid if not then number is out of range
		if(index > history_counter || index < ( -history_counter)) {
			printf("Error: number out of range\n");
			return NULL;
		} else if(index > 0) {
			return history[index-1];
		} else {
			return history[history_counter + index];
		}

	} else {
		printf("Error: Invalid invokation. Try: !! or: !<no>\n");
	}

	return NULL;
}

void add_History(char *inp) { // adds input to history
	
	// new attempt but with shift
	if(history_counter >= 20) { // if history is full
		for(int i = 0; i < history_counter; i++) { // shift everything left
			strcpy(history[i], history[i+1]);
		}
		strcpy(history[history_counter-1], inp);	// then add input at the end
	} else { // else if array is not full
		strcpy(history[history_counter], inp);	// add all of input
		history_counter++; // update count
	}
				
	
}
void print_History() {
	
	// new way of printing with shift in add history
	for(int i = 0; i < history_counter; i++) {

		printf(" %d %s \n", i+1, history[i]);

	}
	
}

void load_history() {

	char buffer[512];
	int i = 0;
	FILE *fp;
	
	//allocates memory to concat the home and history_file
	file = malloc(strlen(getenv("HOME") + strlen(history_file) + 1));
	//copys path to file
	strcpy(file, getenv("HOME"));
	//concatenates the file and history_file 
	strcat(file, history_file);
	

	fp = fopen(file, "r");

	if(fp == NULL) {
		fp = fopen(file, "a");
	}


	while(fgets(buffer, 512, fp) != NULL) {
		add_History(strtok(buffer, "\n"));
	}

	fclose(fp);
	free(file);

}

void save_history() {
	

	//allocates memory to concat the home and history_file
	file = malloc(strlen(getenv("HOME") + strlen(history_file) + 1));
	//copys path to file
	strcpy(file, getenv("HOME"));
	//concatenates the file and history_file 
	strcat(file, history_file);

	FILE *fp = fopen(file,  "w");
	for (int i = 0; i < history_counter; i++) {
		fprintf(fp, "%s\n", history[i]);
	}

	fclose(fp);
	free(file);

}
