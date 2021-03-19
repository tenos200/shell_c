#ifndef max_buffer_size
#define max_buffer_size 512 
#endif

#ifndef alias_file 
#define alias_file "/.aliases"
#endif

#ifndef max_alias_size 
#define max_alias_size 11
#endif


void add_alias(char **tokens, int args);
void remove_alias(char **tokens, int args);
char *invoke_alias(char *fullinp, int invoke, int display);
int number_of_aliases();
void print_alias();
void save_alias();
void load_alias();
void empty_alias();
int number_of_aliases();



//solve this for struct
//
typedef struct {

	char aliasName[max_buffer_size];
	char aliasCommand[max_buffer_size];

} alias;
