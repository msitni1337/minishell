#ifndef LEXER_H
# define LEXER_H

#define DQUOTE '"'
#define SQUOTE '\''

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type 
{
    TOKEN_INVALID,
    TOKEN_EOF,
    TOKEN_CHAR,
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
    char c;
    t_token_type type;
} t_token;

typedef struct s_lexer 
{
    const char *line;
    size_t count;
    size_t pos;
} t_lexer;

// methods:
t_lexer new_lexer(char *line);
t_token get_next_token(t_lexer *lexer, int ignore_spaces);
void parse_line(char *line);

// utils:
void trim_spaces(t_lexer *lexer);


#endif