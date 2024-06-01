#include "shell.h"

void handle_sigint()
{
    g_shell.interrupt = TRUE;
    g_shell.last_exit_value = 130;
    if (g_shell.childs_pids.count > 0)
    {
        write(1, "\n", 1);
        return;
    }
    if (g_shell.collecting_here_doc == TRUE)
    {
        write(1, "\n", 1);
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