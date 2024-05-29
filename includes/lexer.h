#ifndef LEXER_H
#define LEXER_H
#include "shared.h"
#include "shell.h"

#define SYN_ERR "syntax error."
#define SYN_QUOTE "unclosed quote."
#define SYN_BRACE "unclosed brace."

#define DQUOTE '"'
#define SQUOTE '\''
#define IS_CMD_TOKEN(token) ((token).type == TOKEN_STRING || (token).type == TOKEN_REDIRECT_IN || (token).type == TOKEN_REDIRECT_OUT || (token).type == TOKEN_HERE_DOC || (token).type == TOKEN_APPEND || (token).type == TOKEN_OPEN_PAREN)
#define IS_REDIRECT_TOKEN(token) ((token).type == TOKEN_REDIRECT_IN || (token).type == TOKEN_REDIRECT_OUT || (token).type == TOKEN_HERE_DOC || (token).type == TOKEN_APPEND)
#define IS_LOGIC_OP(token) ((token).type == TOKEN_PIPE || (token).type == TOKEN_AND || (token).type == TOKEN_OR)


// Tree Builder:
t_node *create_node(int type);
void init_root(t_node **root);
void append_node(t_node **parent, t_node *child);
void append_child(t_node *parent, t_node *child);
int is_valid_redirect_parent(t_node_type type);
t_node *add_str_node(t_node *root, t_lexer *lexer);
t_node *get_last_node(t_node *node);
t_token *fill_cmd(t_node **root, t_token *token, t_lexer *lexer, int as_child);

// Lexer:
t_node **parse_line(char *line, t_node **root);

// Lexer_utils:
t_node *get_node_by_type(t_node *root, t_node_type type);
t_node *link_new_node(t_node **root, int type, int as_child);
t_node **link_logic_oper(t_node **root, t_lexer *lexer, t_token *token, int as_child);
t_node **init_subshell_node(t_node *root, t_lexer *lexer, t_token *token, t_node **subshell);
t_node *link_argv_node(t_node *curr_cmd, t_lexer *lexer);
void *syntax_error(char *reason);

// tokenizer:
t_token get_next_token(t_lexer *lexer, int ignore_spaces);

// token_utils:
t_lexer new_lexer(char *line);
void trim_spaces(t_lexer *lexer);
int is_special(char c);

#endif