#ifndef LEXER_H
#define LEXER_H
#include "shared.h"

#define DQUOTE '"'
#define SQUOTE '\''
#define IS_CMD_TOKEN(token) (token.type == TOKEN_STRING || token.type == TOKEN_REDIRECT_IN || token.type == TOKEN_REDIRECT_OUT || token.type == TOKEN_HERE_DOC || token.type == TOKEN_APPEND || token.type == TOKEN_OPEN_PAREN)

typedef enum e_token_type
{
    TOKEN_INVALID,
    TOKEN_EOF,
    TOKEN_STRING,
    // TOKEN_SQUOTE,
    // TOKEN_DQUOTE,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HERE_DOC,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
//    TOKEN_SUBSHELL_ARG
} t_token_type;

typedef enum e_node_type
{
    NODE_CMD = (1 << 0),
//    NODE_CMD_ARGS = (1 << 1),
    NODE_STRING = (1 << 1),
    // NODE_SQUOTE,
    // NODE_DQUOTE,
    NODE_PIPE = (1 << 2),
    NODE_REDIRECT_IN = (1 << 3),
    NODE_REDIRECT_OUT = (1 << 4),
    NODE_APPEND = (1 << 5),
    NODE_HERE_DOC = (1 << 6),
    NODE_AND = (1 << 7),
    NODE_OR = (1 << 8),
    NODE_SUBSHELL = (1 << 9),
//    NODE_SUBSHELL_ARG = (1 << 11),
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
    bool has_here_doc;
} t_lexer;

// Tree Builder:
t_node *create_node();
void init_root(t_node **root);
void append_node(t_node **parent, t_node *child);
void append_child(t_node *parent, t_node *child);
int is_valid_redirect_parent(t_node_type type);
t_node *create_node(t_node_type type);
t_node *add_str_node(t_node *root, t_lexer *lexer);
t_node *get_last_node(t_node *node);
t_token fill_cmd(t_node **root, t_token token, t_lexer *lexer, int as_child);

// Lexer:
t_lexer new_lexer(char *line);
t_token get_next_token(t_lexer *lexer, int ignore_spaces);
t_node *parse_line(char *line);

// utils:
void trim_spaces(t_lexer *lexer);
int is_special(char c);

#endif