#include "common.h"

size_t ft_strlen(const char *line)
{
    size_t len;

    len = 0;
    while (line && line[len])
        len++;
    return len;
}

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n' || c == '\f');
}

void ft_putstr(const char *s, int fd)
{
    size_t len;

    len = ft_strlen(s);
    if (s)
        write(fd, s, len);
}

void ft_putendl(const char *s, int fd)
{
    ft_putstr(s, fd);
    write(fd, "\n", 1);
}