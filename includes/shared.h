#ifndef COMMON_H
#define COMMON_H
#define PROMPT "millishell ~>"

#define TRUE 1
#define FALSE 0

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

#define USED(x) ((void)(x))

typedef struct s_string
{
	const char *s;
	size_t count;
} t_string;

typedef struct s_lstenv
{
	char *data;
	struct s_lstenv *next;
} t_lstenv;

typedef struct s_shell
{
	char **exported_env;
	t_lstenv *env_list;
	char *working_dir;
	int last_exit_value;
} t_shell;

// global variable:
extern t_shell shell;

// utils
int ft_isspace(char c);

// freeing
void free_arr(char **arr);

#endif