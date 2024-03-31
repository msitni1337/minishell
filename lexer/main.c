#include "lexer.h"

int main(int c, char **v, char **env) 
{
    USED(c);
    USED(v);
    USED(env);
    
    char *line;

    while ((line = readline(">> ")) != NULL) {
        rl_replace_line("hello", 0);
        printf("line: %s\n", line);
        free(line);
    }
    return 0;
}