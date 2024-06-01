#include "shell.h"
#include "env.h"

void init_shell(const char **envp)
{
    setup_signal_handlers();
    g_shell.last_exit_value = 0;
    g_shell.childs_pids = init_da(sizeof(int)); // TODO : move it to interpreter loop
    take_env(envp);
    g_shell.interrupt = FALSE;
    g_shell.collecting_here_doc = FALSE;
}