#ifndef LEXER_H
#define LEXER_H

#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define DQUOTE '"'
#define SQUOTE '\''
#define NODE_INF -1
#define NODE_CMD_AC NODE_INF
#define NODE_STR_AC 0
#define NODE_REDIRECT_AC 1

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
} t_token_type;

typedef enum e_node_type
{
    NODE_CMD,
    NODE_STR,
    NODE_SQUOTE,
    NODE_DQUOTE,
    NODE_PIPE,
    NODE_REDIRECT_IN,
    NODE_REDIRECT_OUT,
    NODE_APPEND,
    NODE_HERE_DOC,
    NODE_AND_OPERATOR,
    NODE_OR_OPERATOR,
    NODE_OPEN_PAREN,
    NODE_CLOSE_PAREN,
    NODE_OPEN_CURLY,
    NODE_CLOSE_CURLY,
} t_node_type;

typedef struct s_token
{
    char c;
    t_token_type type;
} t_token;

typedef struct s_node
{
    t_node_type type;
    unsigned int args_req;

    t_string token_str;

    struct s_node *next;
    size_t list_count;
    struct s_node *children;
    size_t childs_count;
} t_node;

typedef struct s_lexer
{
    const char *line;
    size_t count;
    size_t pos;
} t_lexer;

// Tree Builder:
t_node *create_node();
void init_root(t_node **root);
t_node *add_dquote_node(t_node **root, t_lexer *lexer, int as_child);
t_node *add_squote_node(t_node **root, t_lexer *lexer, int as_child);
t_node *add_redirect_node(t_node **root, t_lexer *lexer, t_node_type type);
// Lexer:
t_lexer new_lexer(char *line);
t_token get_next_token(t_lexer *lexer, int ignore_spaces);
void parse_line(char *line);
t_string get_dquote_string(t_lexer *lexer);
t_string get_squote_string(t_lexer *lexer);
t_string get_string_delim(t_lexer *lexer, const char delim);
t_string get_string_whitespace(t_lexer *lexer);

// utils:
void trim_spaces(t_lexer *lexer);

#endif