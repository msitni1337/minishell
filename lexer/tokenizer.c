#include "lexer.h"

t_token get_next_token(t_lexer*lexer)
{
    t_token token;
    char current;

    trim_spaces(lexer);
    current = lexer->line[lexer->pos];
    token.type = TOKEN_INVALID;
    if (!current || lexer->pos >= lexer->count)
        token.type = TOKEN_EOF;
    else if (current == '>' && lexer->line[lexer->pos + 1] == '>')
        token.type = TOKEN_APPEND;
    else if (current == '<' && lexer->line[lexer->pos + 1] == '<')
        token.type = TOKEN_HERE_DOC;
    else if (current == '>')
        token.type = TOKEN_REDIRECT_OUT;
    else if (current == '<')
        token.type = TOKEN_REDIRECT_IN;
    else if (current == '"')
        token.type = TOKEN_DQUOTE;
    else if (current == '\'')
        token.type = TOKEN_SQUOTE;
    else if (current == '\'')
        token.type = TOKEN_SQUOTE;
    else if (current == '(')
        token.type = TOKEN_OPEN_PAREN;
    else if (current == ')')
        token.type = TOKEN_CLOSE_PAREN;
    else if (current == '{')
        token.type = TOKEN_OPEN_CURLY;
    else if (current == '}')
        token.type = TOKEN_CLOSE_CURLY;
    else if (current == '$')
        token.type = TOKEN_DOLLAR;
    else if (current == '&' && lexer->line[lexer->pos + 1] == '&')
        token.type = TOKEN_AND_OPERATOR;
    else if (current == '|' && lexer->line[lexer->pos + 1] == '|')
        token.type = TOKEN_OR_OPERATOR;
    else if (current == '|')
        token.type = TOKEN_PIPE;
    else
        token.type = TOKEN_CHAR;
    return token;
}