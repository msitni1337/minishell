#include "shell.h"

void handle_sigint()
{
    write(0, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void setup_signal_handlers()
{
    // TODO: Handle Loging error before Exiting..
    if (signal(SIGINT, &handle_sigint) == SIG_ERR)
        exit(1);
    if (signal(SIGQUIT, SIG_IGN)== SIG_ERR)
        exit(1);
}