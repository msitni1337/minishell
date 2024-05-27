#include "free.h"

void free_p(void*p1, void*p2, void*p3, char**p4)
{
    if (p1)
        free(p1);
    if (p2)
        free(p2);
    if (p3)
        free(p3);
    if (p4)
        free_arr(p4);
    
}

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
    free_p(shell.line, shell.childs_pids.data, NULL, NULL);
    shell.line = NULL;
    shell.childs_pids.data = NULL;
    shell.childs_pids.capacity = 0;
    free_env_list(shell.env_list);
    free_tree(&shell.tree_root);
    close_here_docs();
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

void free_cmd(t_cmd*cmd)
{
    char *argv0;

    argv0 = cmd->argv[0];
    free_arr(cmd->argv);
    if (argv0 != cmd->bin_path)
        free(cmd->bin_path);
}