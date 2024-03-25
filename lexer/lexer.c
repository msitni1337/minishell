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

t_token get_next_token(t_lexer*lexer)
{
    t_token token;
    char current;

    trim_spaces(lexer);
    current = lexer->line[lexer->pos];
    if (!current)
        token.type = TOKEN_EOF;
    else if (current == '>')
        token.type = TOKEN_REDIRECT_OUT;
    else if (current == '<')
        token.type = TOKEN_REDIRECT_IN;
    
    return token;
}