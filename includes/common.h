#ifndef COMMON_H
#define COMMON_H
#define PROMPT "millishell ~>"

#define TRUE    1
#define FALSE   0

#include <stddef.h>
#include <assert.h>
#include <unistd.h>
#include "libft.h"

#define USED(x) ((void)(x))

typedef struct s_string
{
    const char*s;
    size_t count;
}   t_string;

typedef struct s_redirect
{
    t_string value;
    int append;
} t_redirect;

typedef struct s_cmd
{
    char* bin_path;
    char** args;

    char **here_doc;
    t_redirect *infile;
    t_redirect *outfile;

} t_cmd;

int ft_isspace(char c);

// freeing
void free_cmd(t_cmd cmd);
void free_arr(char**arr);

#endif