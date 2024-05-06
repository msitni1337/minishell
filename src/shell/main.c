#include "shell.h"

int main(int c, char **v, char **env)
{
    (void) c;
    (void) v;

    init_shell(env);
    start_shell();
    ft_putendl_fd("exit", 1);
    return shell.last_exit_value;
}