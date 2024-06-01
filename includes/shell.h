#ifndef SHELL_H
# define SHELL_H
# include "dynamic_arrays.h"
# include "shared.h"
# include <signal.h>

typedef struct s_shell
{
	t_lstenv	*env_list;
	int			last_exit_value;
	int			interrupt;
	int			collecting_here_doc;
	t_darr		childs_pids;
	t_node		*tree_root;
	char		*line;
}				t_shell;

// global variable:
extern t_shell	g_shell;

void			execute_file(void);
void			start_shell(void);
void			init_shell(const char **envp);
void			setup_signal_handlers(void);

#endif