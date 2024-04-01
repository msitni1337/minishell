#ifndef COMMON_H
#define COMMON_H
#define PROMPT "millishell ~>"

#define TRUE    1
#define FALSE   0

#include <stddef.h>
#include <assert.h>
#include <unistd.h>

#define USED(x) ((void)(x))

typedef struct s_string
{
    const char*s;
    size_t count;
}   t_string;

typedef struct s_cmd
{
    char* bin_path;
    char** args;
    int infile;
    int is_here_doc;
    int outfile;
    int append;
} t_cmd;

int ft_isspace(char c);
size_t ft_strlen(const char *line);
void ft_putstr(const char *s, int fd);
void ft_putendl(const char *s, int fd);

// freeing
void free_cmd(t_cmd cmd);
void free_arr(char**arr);

#endif