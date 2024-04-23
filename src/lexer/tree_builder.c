#include "lexer.h"

t_node *create_node(t_node_type type)
{
    t_node *res;

    res = malloc(sizeof(t_node));
    if (!res)
        return NULL; // TODO: free & exit with malloc error
    ft_memset(res, 0, sizeof(t_node));
    // res->args_req = args_req;
    res->type = type;
    return res;
}

t_node *get_last_node(t_node *node)
{
    t_node *last;

    last = node;
    while (last && last->next)
        last = last->next;
    return last;
}

void append_node(t_node **parent, t_node *child)
{
    t_node *node;

    if (*parent == NULL)
    {
        *parent = child;
        child->list_count++;
        return;
    }
    (*parent)->list_count++;
    node = *parent;
    while (node->next)
        node = node->next;
    node->next = child;
}

void append_child(t_node *parent, t_node *child)
{
    t_node **tmp;
    t_node *node;

    if (parent == NULL)
        return;
    parent->childs_count++;
    tmp = &(parent->children);
    node = parent->children;
    while (node)
    {
        tmp = &(node->next);
        node = node->next;
    }
    *tmp = child;
}

// todo: no need for passing as_child arg ?
t_node *add_dquote_node(t_node *root, t_lexer *lexer)
{
    t_node *node;

    node = create_node(NODE_DQUOTE);
    node->token_str = get_string_delim(lexer, DQUOTE);
    append_child(root, node);
    if (lexer->pos < lexer->count && !ft_isspace(lexer->line[lexer->pos]))
    {
        if (lexer->line[lexer->pos] == DQUOTE)
            add_dquote_node(node, lexer);
        else if (lexer->line[lexer->pos] == SQUOTE)
            add_squote_node(node, lexer);
        else
            add_str_node(node, lexer);
    }
    return node;
}

t_node *add_squote_node(t_node *root, t_lexer *lexer)
{
    t_node *node;

    node = create_node(NODE_SQUOTE);
    node->token_str = get_string_delim(lexer, SQUOTE);
    append_child(root, node);
    if (lexer->pos < lexer->count && !ft_isspace(lexer->line[lexer->pos]))
    {
        if (lexer->line[lexer->pos] == DQUOTE)
            add_dquote_node(node, lexer);
        else if (lexer->line[lexer->pos] == SQUOTE)
            add_squote_node(node, lexer);
        else
            add_str_node(node, lexer);
    }
    return node;
}

t_node *add_str_node(t_node *root, t_lexer *lexer)
{
    t_node *node;

    node = create_node(NODE_STRING);
    node->token_str = get_string_whitespace(lexer);
    append_child(root, node);

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