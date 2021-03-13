#ifndef history_file
#define history_file "/.hist_list"
#endif
#ifndef max_buffer_size
#define max_buffer_size 512 
#endif

void add_History(char *inp);
void save_history();
void load_history();
void print_History();
char *invoke_History(char *inp);
