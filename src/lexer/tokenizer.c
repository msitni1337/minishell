#include "lexer.h"

void get_next_token_1(t_lexer *lexer, t_token *token)
{
    if (!token->c || lexer->pos >= lexer->count)
        token->type = TOKEN_EOF;
    else if (token->c == '>' && lexer->line[lexer->pos + 1] == '>')
    {
        token->type = TOKEN_APPEND;
        lexer->pos += 2;
    }
    else if (token->c == '<' && lexer->line[lexer->pos + 1] == '<')
    {
        token->type = TOKEN_HERE_DOC;
        lexer->pos += 2;
    }
    else if (token->c == '>')
    {
        token->type = TOKEN_REDIRECT_OUT;
        lexer->pos++;
    }
    else if (token->c == '<')
    {
        token->type = TOKEN_REDIRECT_IN;
        lexer->pos++;
    }
}

void get_next_token_2(t_lexer *lexer, t_token *token)
{
    if (token->c == '(')
    {
        token->type = TOKEN_OPEN_PAREN;
        lexer->pos++;
    }
    else if (token->c == ')')
    {
        token->type = TOKEN_CLOSE_PAREN;
        lexer->pos++;
    }
    else if (token->c == '&' && lexer->line[lexer->pos + 1] == '&')
    {
        token->type = TOKEN_AND;
        lexer->pos += 2;
    }
    else if (token->c == '|' && lexer->line[lexer->pos + 1] == '|')
    {
        token->type = TOKEN_OR;
        lexer->pos += 2;
    }
    else if (token->c == '|')
    {
        token->type = TOKEN_PIPE;
        lexer->pos++;
    }
}

t_token get_next_token(t_lexer *lexer, int ignore_spaces)
{
    t_token token;

    if (ignore_spaces)
        trim_spaces(lexer);
    token.c = lexer->line[lexer->pos];
    if (!token.c || token.c == '>' || token.c == '<')
        get_next_token_1(lexer, &token);
    else if(token.c == '(' || token.c == ')' || token.c == '|' || token.c == '&')
        get_next_token_2(lexer, &token);
    else
    {
        token.type = TOKEN_STRING;
        lexer->pos++;
    }
    return token;
}