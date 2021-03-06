void add_alias(char **tokens, int args);
void remove_alias(char **tokens, int args);
void invoke_alias();
void print_alias();
char *concat_str();

static const int max_alias_size = 10;

//solve this for struct
//
typedef struct {

	char aliasName[512];
	char aliasCommand[512];

} alias;

static alias alias_map[max_alias_size];
