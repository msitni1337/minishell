#include "lexer.h"
#include "test.h"
#include "dynamic_arrays.h"

void handle_sigint()
{
    printf("Intercepted\n");
}

void setup_signal_handlers()
{
    signal(SIGINT, &handle_sigint);
}


void init_shell()
{
    setup_signal_handlers();
}

int main(int c, char **v, char **env)
{
    t_node *root;
    USED(c);
    USED(v);
    USED(env);

    init_shell();
    char *line;

    line = readline(PROMPT);
    while (line != NULL)
    {
        root = parse_line(line);

        print_tree(root);

        free(line);
        line = readline(PROMPT);
    }
    return 0;
}