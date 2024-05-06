#include "shell.h"

void init_shell(char**envp)
{
    setup_signal_handlers();
    shell.last_exit_value = 0;
    shell.exported_env = envp;
}