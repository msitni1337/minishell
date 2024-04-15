#include "lexer.h"

t_node *create_node(t_node_type type)
{
    t_node *res;

    res = malloc(sizeof(t_node));
    if (!res)
        return NULL; // TODO: free & exit with malloc error
    ft_memset(res, 0, sizeof(t_node));
    //res->args_req = args_req;
    res->type = type;
    return res;
}

t_node*get_last_node(t_node*node)
{
    t_node*last;

    last = node;
    while (last && last->next)
        last = last->next;
    return last;    
}

void append_node(t_node **parent, t_node *child, size_t *count)
{
    t_node *node;

    (*count)++;
    if (*parent == NULL)
    {
        *parent = child;
        return;
    }
    node = *parent;
    while (node->next)
        node = node->next;
    node->next = child;
}

// todo: no need for passing as_child arg ?
t_node *add_dquote_node(t_node **root, t_lexer *lexer, int as_child)
{
    t_node *node;

    node = create_node(NODE_DQUOTE);
    node->token_str = get_string_delim(lexer, DQUOTE);
    if (as_child)
        append_node(&((*root)->children), node, &((*root)->childs_count));
    else
        append_node(&((*root)->next), node, &((*root)->list_count));
    return node;
}

t_node *add_squote_node(t_node **root, t_lexer *lexer, int as_child)
{
    t_node *node;

    node = create_node(NODE_SQUOTE);
    node->token_str = get_string_delim(lexer, SQUOTE);
    if (as_child)
        append_node(&((*root)->children), node, &((*root)->childs_count));
    else
        append_node(&((*root)->next), node, &((*root)->list_count));
    return node;
}

t_node *add_str_node(t_node **root, t_lexer *lexer, int as_child)
{
    t_node *node;

    node = create_node(NODE_STRING);
    node->token_str = get_string_whitespace(lexer);
    if (as_child)
        append_node(&((*root)->children), node, &((*root)->childs_count));
    else
        append_node(&((*root)->next), node, &((*root)->list_count));
    return node;
}

/*
t_node *add_redirect_node(t_node **root, t_lexer *lexer, t_node_type type)
{
    t_node *node;

    node = create_node(type);
    append_node(&((*root)->next), node, &((*root)->childs_count));
    return node;
}
*/