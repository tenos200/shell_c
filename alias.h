#ifndef max_buffer_size
#define max_buffer_size 512 
#endif

#ifndef max_alias_size 
#define max_alias_size 10
#endif
void add_alias(char **tokens, int args);
void remove_alias(char **tokens, int args);
char *invoke_alias(char *fullinp);
void print_alias();


//solve this for struct
//
typedef struct {

	char aliasName[max_buffer_size];
	char aliasCommand[max_buffer_size];

} alias;

static alias alias_map[max_alias_size];
