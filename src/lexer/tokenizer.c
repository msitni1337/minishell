#include "lexer.h"

t_token get_next_token(t_lexer *lexer, int ignore_spaces)
{
    t_token token;
    char current;

    if (ignore_spaces)
        trim_spaces(lexer);
    current = lexer->line[lexer->pos];
    token.type = TOKEN_INVALID;
    if (!current || lexer->pos >= lexer->count)
        token.type = TOKEN_EOF;
    else if (current == '>' && lexer->line[lexer->pos + 1] == '>')
    {
        token.type = TOKEN_APPEND;
        lexer->pos += 2;
    }
    else if (current == '<' && lexer->line[lexer->pos + 1] == '<')
    {
        token.type = TOKEN_HERE_DOC;
        lexer->pos += 2;
    }
    else if (current == '>')
    {
        token.type = TOKEN_REDIRECT_OUT;
        lexer->pos++;
    }
    else if (current == '<')
    {
        token.type = TOKEN_REDIRECT_IN;
        lexer->pos++;
    }
    else if (current == DQUOTE)
    {
        token.type = TOKEN_DQUOTE;
        lexer->pos++;
    }
    else if (current == SQUOTE)
    {
        token.type = TOKEN_SQUOTE;
        lexer->pos++;
    }
    else if (current == '(')
    {
        token.type = TOKEN_OPEN_PAREN;
        lexer->pos++;
    }
    else if (current == ')')
    {
        token.type = TOKEN_CLOSE_PAREN;
        lexer->pos++;
    }
    else if (current == '&' && lexer->line[lexer->pos + 1] == '&')
    {
        token.type = TOKEN_AND;
        lexer->pos += 2;
    }
    else if (current == '|' && lexer->line[lexer->pos + 1] == '|')
    {
        token.type = TOKEN_OR;
        lexer->pos += 2;
    }
    else if (current == '|')
    {
        token.type = TOKEN_PIPE;
        lexer->pos++;
    }
    else
    {
        token.type = TOKEN_STRING;
        lexer->pos++;
    }
    return token;
}