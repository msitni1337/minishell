#include "lexer.h"
#include "dynamic_arrays.h"

int main(int c, char **v, char **env)
{
    USED(c);
    USED(v);
    USED(env);

    char *line;

    line = readline(PROMPT);
    while (line != NULL)
    {
        parse_line(line);
        free(line);
        line = readline(PROMPT);
    }
    return 0;
}