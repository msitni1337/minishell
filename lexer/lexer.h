#ifndef LEXER_H
# define LEXER_H

#define USED(x) ((void)(x))

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type 
{
    TOKEN_INVALID,
    TOKEN_EOF,
    TOKEN_STRING,
    TOKEN_SQUOTE,
    TOKEN_DQUOTE,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HERE_DOC,
    TOKEN_AND_OPERATOR,
    TOKEN_OR_OPERATOR,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_CURLY,
    TOKEN_CLOSE_CURLY,
    TOKEN_DOLLAR,
    TOKEN_UNKNOWN
} t_token_type;

typedef struct s_token 
{
    char *value;
    t_token_type type;
} t_token;

typedef struct s_lexer 
{
    char *line;
    int count;
    int pos;
} t_lexer;

#endif