#include "interpreter.h"

void print_tree(t_node *root);

// global variable:
t_shell shell;

void start_shell()
{
    t_node* cmd_root;
    char * line;

    line = readline(PROMPT);
    while (line != NULL)
    {
        cmd_root = parse_line(line);

        if (cmd_root)
            print_tree(cmd_root);
        free(line);
        line = readline(PROMPT);
        free(line);
        if (cmd_root)
            shell.last_exit_value = interpret_root(cmd_root);

        line = readline(PROMPT);
    }
}