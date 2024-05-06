#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "shared.h"
#include "lexer.h"
#include "cmd.h"
#include "built-ins.h"
#include "env.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>



//  interpreter:
int interpret_root(t_node *root);

//  execution:
int execute_cmd(t_cmd cmd, int wait);

//  utils:
t_node *get_next_node_by_type(t_node *root, t_node_type type);


#endif