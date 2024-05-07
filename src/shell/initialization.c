#include "shell.h"
#include "env.h"

void init_shell(char**envp)
{
    setup_signal_handlers();
    shell.last_exit_value = 0;
    shell.exported_env = envp;
    shell.childs_pids = init_da(sizeof(int), NULL);
    take_env(&(shell.env_list), (const char**)envp);
}