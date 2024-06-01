#include "built_ins.h"

int check_key_is_valid(char*key)
{
    size_t i;

    i = 0;
    if (!ft_isalpha(key[i]) && key[i] != '_')
        return 1;
    i++;
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return 1;
        i++;
    }
    return 0;
}

int key_not_valid(char *cmd, char*key)
{
    ft_putstr_fd(PROG_NAME": " , STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putstr_fd(": `" , STDERR_FILENO);
    ft_putstr_fd(key , STDERR_FILENO);
    ft_putendl_fd("': not a valid identifier" , STDERR_FILENO);
    return 1;
}