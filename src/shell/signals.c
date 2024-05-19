#include "shell.h"

void handle_sigint()
{
    shell.interrupt = TRUE;
    shell.last_exit_value = 130;
    if (shell.childs_pids.count > 0)
    {
        write(1, "\n", 1);
        return;
    }
    if (shell.collecting_here_doc == TRUE)
    {
        close(0);
        return;
    }
    rl_on_new_line();
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_redisplay();
}

void setup_signal_handlers()
{
    // TODO: Handle Loging error before Exiting..
    if (signal(SIGINT, handle_sigint) == SIG_ERR)
        exit(1);
    if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
        exit(1);
}