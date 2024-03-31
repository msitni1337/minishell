#include "lexer.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t'
        || c == '\v' || c == '\r'
        || c == '\n' || c == '\f');
}

void trim_spaces(t_lexer *lexer)
{
    while (lexer->pos < lexer->count && ft_isspace(lexer->line[lexer->pos]))
        lexer->pos++;
}