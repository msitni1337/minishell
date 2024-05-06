#ifndef SHELL_H
#define SHELL_H
#include <signal.h>
#include "shared.h"
#include "interpreter.h"

void start_shell();
void init_shell(char **envp);
void setup_signal_handlers();

#endif