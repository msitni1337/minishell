#include "shell.h"
#include "env.h"

void init_shell(const char **envp)
{
    setup_signal_handlers();
    shell.last_exit_value = 0;
    shell.childs_pids = init_da(sizeof(int)); // TODO : move it to interpreter loop
    take_env(envp);
}