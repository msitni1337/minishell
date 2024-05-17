#include "built-ins.h"

void print_error(char *arg, char*reason)
{
    ft_putstr_fd(PROG_NAME": exit: " , STDERR_FILENO);
    if (arg)
    {
        ft_putstr_fd(arg , STDERR_FILENO);
        ft_putstr_fd(": " , STDERR_FILENO);
    }
    ft_putendl_fd(reason , STDERR_FILENO);
}

int is_str_all_digits(char*s)
{
    size_t i;

    i = 0;
    while (s && s[i])
    {
        if (s[i] >= '0' && s[i] >= '9')
            return 0;
        i++;
    }
    return 1;
}

int ft_exit(t_cmd cmd)
{
    int ret_value;

    ret_value = shell.last_exit_value;
    if (cmd.outfile == STDOUT_FILENO)
        ft_putendl_fd("exit", cmd.outfile);
    else
    {
        close(cmd.outfile);
    }
    if (cmd.argc > 1)
    {
        if (is_str_all_digits(cmd.argv[1]))
        {
            if (cmd.argc == 2)
            {
                print_error(NULL, "too many arguments");
                return 1;
            }
            ret_value = ft_atoi(cmd.argv[1]);
        }
        else
        {
            ret_value = 2;
            print_error(cmd.argv[1], "numeric argument required");
        }
    }
    exit(ret_value);
    return 0;
}