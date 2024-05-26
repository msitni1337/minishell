#include "free.h"

void free_childs(t_node *parent)
{
    t_node *tmp;
    t_node *node;

    node = parent->children;
    while (node)
    {
        if (node->children)
            free_childs(node);
        tmp = node->next;
        free(node);
        node = tmp;
    }
}

void free_tree(t_node **root)
{
    t_node *tmp;
    t_node *node;

    node = *root;
    while (node)
    {
        if (node->children)
            free_childs(node);
        tmp = node->next;
        free(node);
        node = tmp;
    }
    *root = NULL;
}

void free_env_list(t_lstenv *head)
{
    t_lstenv*node;
    t_lstenv*tmp;

    node = head;
    while (node)
    {
        if (node->key)
            free(node->key);
        if (node->value)
            free(node->value);
        tmp = node->next;
        free(node);
        node = tmp;
    }
}

void free_global_vars()
{
    if (shell.childs_pids.data)
        free(shell.childs_pids.data);
    free_env_list(shell.env_list);
}

void free_arr(char **arr)
{
    char **tmp;

    tmp = arr;
    while (arr && *arr)
    {
        free(*arr);
        arr++;
    }
    if (arr)
        free(tmp);
}

void close_here_docs()
{
    struct stat s;
    int fd;

    fd = 3;
    while (fd > 2 && fstat(fd, &s) != -1 && errno != EBADF)
    {
        close(fd);
        fd++;
    }
}