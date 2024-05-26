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

// utils
int ft_isspace(char c);

// freeing
void free_arr(char **arr);
void close_here_docs();
void malloc_error(t_cmd*cmd);
void exit_with_code(t_cmd*cmd, int exit_value);

#endif