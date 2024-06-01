#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "built_ins.h"
# include "env.h"
# include "free.h"
# include "lexer.h"
# include "shared.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_expansion_state
{
	NORMAL,
	DQUOTE_STATE,
	SQUOTE_STATE,
}		t_expansion_state;

//  interpreter:
int		interpret_root(t_node *root);

//  execution:
int		execute_cmd(t_cmd *cmd, int is_pipe, int wait_child);
int		wait_all_childs(void);

//  utils:
t_node	*get_next_node_by_type(t_node *root, t_node_type type);
int		contains_chars(t_string string, char *charset);

/* asterices expansion */
char	**expand_asterices(char **argv, size_t *argc);

/* string expansion */
char	*expand_string(t_string string, int expand_vars);
size_t	get_expanded_str_len(t_string string, int expand_vars);
size_t	parse_key_count(const char *s);
void	copy_var_value(char *res, t_string *string, size_t *i);
int		count_num_chars(long n);

/* errors */
void	print_error(char *name, char *reason);

#endif