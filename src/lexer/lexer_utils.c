#include "lexer.h"

t_node *get_node_by_type(t_node *root, t_node_type type)
{
    t_node *tmp;

    tmp = root;
    if (!tmp)
        return NULL;
    tmp = root->children;
    while (tmp && tmp->type != type)
        tmp = tmp->next;
    return tmp;
}

t_node *link_new_node(t_node **root, int type, int as_child)
{
    t_node *node;

    node = create_node(type);
    if (as_child)
        append_child(*root, node);
    else
        append_node(root, node);
    return node;
}

t_node **link_logic_oper(t_node **root, t_lexer *lexer, t_token *token, int as_child)
{
    link_new_node(root, token->type, as_child);
    *token = get_next_token(lexer, TRUE);
    if (IS_CMD_TOKEN(*token))
        return root;
    return NULL;
}

t_node **init_subshell_node(t_node *root, t_lexer *lexer, t_token *token, t_node **subshell)
{
    *subshell = create_node(NODE_SUBSHELL);
    append_child(root, *subshell);
    *token = get_next_token(lexer, TRUE);
    if (token->type == TOKEN_EOF)
        return syntax_error(SYN_BRACE);
    return subshell;
}

t_node *link_argv_node(t_node *curr_cmd, t_lexer *lexer)
{
    if (add_str_node(curr_cmd, lexer) == NULL)
        return syntax_error(SYN_QUOTE);
    if (get_node_by_type(curr_cmd, NODE_SUBSHELL) != NULL)
        return syntax_error("CMD ARGS AFTER SUBSHELL");
    return curr_cmd;
}

void *syntax_error(char *reason)
{
    ft_putstr_fd(PROG_NAME": ", STDERR_FILENO);
    ft_putstr_fd(reason, STDERR_FILENO);
    write(STDERR_FILENO, "\n", 1);
    return NULL;
}
