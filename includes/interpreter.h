#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "shared.h"
#include "lexer.h"
#include "cmd.h"
#include "built-ins.h"
#include "env.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

typedef enum e_expansion_state
{
    NORMAL,
    DQUOTE_STATE,
    SQUOTE_STATE,
} t_expansion_state;

//  interpreter:
int interpret_root(t_node *root);

//  execution:
int execute_cmd(t_cmd cmd, bool is_pipe);
int wait_all_childs();

//  utils:
t_node *get_next_node_by_type(t_node *root, t_node_type type);

char *expand_string(t_string string, int expand_vars);

#endif