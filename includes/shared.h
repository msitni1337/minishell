#ifndef COMMON_H
#define COMMON_H
#define PROMPTSTART "\e[1;32mMillishell \e[0;37m[\e[1;34m"
#define PROMPTEND "\e[0;37m]\e[0;32m~> \e[0;37m"

#define bool unsigned char
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
	bool is_set;
	char *key;
	char *value;
	struct s_lstenv *next;
} t_lstenv;

// utils
int ft_isspace(char c);

// freeing
void free_arr(char **arr);

#endif