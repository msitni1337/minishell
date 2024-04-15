#ifndef LEXER_H
#define LEXER_H

#include "dynamic_arrays.h"
#include "shared.h"

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define DQUOTE '"'
#define SQUOTE '\''

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
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_CURLY,
    TOKEN_CLOSE_CURLY,
    TOKEN_DOLLAR,
} t_token_type;

typedef enum e_node_type
{
    NODE_CMD,
    NODE_CMD_ARGS,
    NODE_STRING,
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
    // unsigned int args_req;

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
// t_node *add_redirect_node(t_node **root, t_lexer *lexer, t_node_type type);
int is_valid_redirect_parent(t_node_type type);
t_node *create_node(t_node_type type);
void append_node(t_node **parent, t_node *child, size_t *count);
t_node *add_dquote_node(t_node **root, t_lexer *lexer, int as_child);
t_node *add_squote_node(t_node **root, t_lexer *lexer, int as_child);
t_node *add_str_node(t_node **root, t_lexer *lexer, int as_child);
t_node*get_last_node(t_node*node);

// Lexer:
t_lexer new_lexer(char *line);
t_token get_next_token(t_lexer *lexer, int ignore_spaces);
t_node* parse_line(char *line);
t_string get_dquote_string(t_lexer *lexer);
t_string get_squote_string(t_lexer *lexer);
t_string get_string_delim(t_lexer *lexer, const char delim);
t_string get_string_whitespace(t_lexer *lexer);

// utils:
void trim_spaces(t_lexer *lexer);

#endif