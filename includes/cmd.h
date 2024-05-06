#ifndef CMD_H
#define CMD_H
#include "lexer.h"

typedef struct s_cmd
{
	int is_subshell;
	int is_builtin;
	int infile;
	int outfile;
	size_t argc;
	char **argv;
	char *binary;
	t_node *subshell;
} t_cmd;

#endif