#ifndef COMMON_H
#define COMMON_H
#define PROG_NAME "Millishell"
#define PROMPTSTART "\001\e[1;32m\002"PROG_NAME" \001\e[0;37m\002[\001\e[1;34m\002"
#define PROMPTEND "\001\e[0;37m\002]\001\e[0;32m\002~> \001\e[0;37m\002"

#define bool unsigned char
#define TRUE 1
#define FALSE 0

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

#define USED(x) ((void)(x))
#define eq(c1, c2) ((c1) == (c2))

typedef struct s_string
{
	const char *s;
	size_t count;
} t_string;

typedef struct s_lstenv
{
	bool is_set;
	char *key;
	char *value;
	struct s_lstenv *next;
} t_lstenv;

typedef enum e_token_type
{
    TOKEN_EOF = 0,
    TOKEN_STRING = (1 << 1),
    TOKEN_PIPE = (1 << 2),
    TOKEN_REDIRECT_IN = (1 << 3),
    TOKEN_REDIRECT_OUT = (1 << 4),
    TOKEN_APPEND = (1 << 5),
    TOKEN_HERE_DOC = (1 << 6),
    TOKEN_AND  = (1 << 7),
    TOKEN_OR  = (1 << 8),
    TOKEN_OPEN_PAREN = (1 << 9),
    TOKEN_CLOSE_PAREN  = (1 << 10),
} t_token_type;

typedef enum e_node_type
{
    NODE_CMD = (1 << 0),
    NODE_STRING = (1 << 1),
    NODE_PIPE = (1 << 2),
    NODE_REDIRECT_IN = (1 << 3),
    NODE_REDIRECT_OUT = (1 << 4),
    NODE_APPEND = (1 << 5),
    NODE_HERE_DOC = (1 << 6),
    NODE_AND = (1 << 7),
    NODE_OR = (1 << 8),
    NODE_SUBSHELL = (1 << 9),
} t_node_type;

typedef struct s_token
{
    t_token_type type;
    char c;
} t_token;

typedef struct s_node
{
    t_node_type type;

    t_string token_str;
    struct s_node *next;
    size_t list_count;
    struct s_node *children;
    size_t childs_count;
    int here_doc_fd;
} t_node;

typedef struct s_lexer
{
    const char *line;
    size_t count;
    size_t pos;
} t_lexer;

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
    int read_pipe;
    size_t argc;
    char **argv;
    char *bin_path;
    t_node *subshell;
    // t_node **tree_root;
} t_cmd;


// utils
int ft_isspace(char c);

// freeing
void free_arr(char **arr);
void free_global_vars();
void free_tree(t_node **root);
void close_here_docs();
void malloc_error(void*p1, void*p2, char**p3, t_cmd*cmd);
void exit_with_code(t_cmd*cmd, int exit_value);
void *free_p(void*p1, void*p2, void*p3, char**p4);

#endif