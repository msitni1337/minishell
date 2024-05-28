#include "lexer.h"

t_node *create_node(int type)
{
    t_node *res;

    res = malloc(sizeof(t_node));
    if (res == NULL)
        return NULL;
    ft_memset(res, 0, sizeof(t_node));
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