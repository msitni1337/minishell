#ifndef SHELL_H
#define SHELL_H
#include <signal.h>
#include "shared.h"
#include "dynamic_arrays.h"


typedef struct s_shell
{
	t_lstenv *env_list;
	char *working_dir;
	int last_exit_value;
	int interrupt;
	int collecting_here_doc;
    t_darr childs_pids;
	t_node*tree_root;
	char*line;
} t_shell;

// global variable:
extern t_shell shell;

void execute_file();
void start_shell();
void init_shell(const char **envp);
void setup_signal_handlers();

#endif