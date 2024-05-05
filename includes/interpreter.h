#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "lexer.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

typedef struct s_cmd
{
    int is_subshell;
    int is_builtin;
    int infile;
    int outfile;
    size_t argc;
    char**argv;
    t_node* subshell;
} t_cmd;

//  interpreter:
int interpret_root(t_node *root);


//  execution:
int execute_cmd(t_cmd cmd);


//  utils:
t_node *get_next_node_by_type(t_node *root, t_node_type type);


#endif