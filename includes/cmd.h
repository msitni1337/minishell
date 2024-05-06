#ifndef CMD_H
#define CMD_H
#include "lexer.h"

typedef enum e_cmd_type
{
    CMD_BINARY,
    CMD_SUBSHELL,
    CMD_BUILTIN
} t_cmd_type;

typedef struct s_cmd
{
    t_cmd_type type;
    int infile;
    int outfile;
    size_t argc;
    char **argv;
    char *bin_path;
    t_node *subshell;
} t_cmd;

#endif