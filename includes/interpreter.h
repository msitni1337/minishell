#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "shared.h"
#include "lexer.h"
#include "cmd.h"
#include "built-ins.h"
#include "env.h"
#include "error_handler.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

typedef enum e_expansion_state
{
    NORMAL,
    DQUOTE_STATE,
    SQUOTE_STATE,
} t_expansion_state;

//  interpreter:
int interpret_root(t_node *exec_root, t_node**tree_root);

//  execution:
int execute_cmd(t_cmd cmd, bool is_pipe, bool wait_child);
int wait_all_childs();

//  utils:
t_node *get_next_node_by_type(t_node *root, t_node_type type);
bool contains_chars(t_string string, char *charset);

/* asterices expansion */
char **expand_asterices_argv(char **argv, size_t *argc);

/* string expansion */
char *expand_string(t_string string, int expand_vars);
size_t get_expanded_str_len(t_string string, int expand_vars);
size_t parse_key_count(const char *s);
void copy_var_value(char *res, t_string *string, size_t *i);

#endif