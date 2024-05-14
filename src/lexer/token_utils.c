#include "lexer.h"

t_lexer new_lexer(char *line)
{
    t_lexer lexer;

    lexer.line = line;
    lexer.count = ft_strlen(line);
    lexer.pos = 0;
    return lexer;
}

void trim_spaces(t_lexer *lexer)
{
    while (lexer->pos < lexer->count && ft_isspace(lexer->line[lexer->pos]))
        lexer->pos++;
}

int is_special(char c)
{
    return (c == '(' || c == ')' || c == '<' ||
     c == '>' || c == '|');
}