#include "shell.h"

t_shell g_shell;

char **expand_asterices_argv(char **argv, size_t *argc);

int main(int c, const char **v, const char **env)
{
    (void)c;
    (void)v;

    init_shell(env);
    if (isatty(0) == 1)
        start_shell();
    else
        execute_file();
    return g_shell.last_exit_value;
}