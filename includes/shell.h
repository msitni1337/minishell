#ifndef SHELL_H
#define SHELL_H
#include <signal.h>
#include "shared.h"
#include "dynamic_arrays.h"


typedef struct s_shell
{
	char **exported_env;
	t_lstenv *env_list;
	char *working_dir;
	int last_exit_value;
    t_darr childs_pids;
} t_shell;

// global variable:
extern t_shell shell;

void start_shell();
void init_shell(const char **envp);
void setup_signal_handlers();

#endif