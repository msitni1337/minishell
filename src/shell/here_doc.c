#include "interpreter.h"
/*
void get_here_doc(t_node *node)
{
    int pip[2];
    char *line;
    char *delim;

    node->here_doc_fd = -1;
    delim = expand_string(node->token_str, FALSE);
    if (pipe(pip) == -1)
    {
        perror("pipe");
        return;
    }
    line = readline(">");
    node->here_doc_fd = pip[0];
    while (line)
    {
        if (ft_strcmp(line, delim) == 0)
        {
            close(pip[1]);
            free(line);
            return;
        }
        ft_putendl_fd(line, pip[1]);
        free(line);
        line = readline(">");
    }
    // error -> bash: warning: here-document at line 13 delimited by end-of-file (wanted `hh')
    close(pip[1]);
    perror(delim);
}

void collect_cmd_here_doc(t_node *cmd)
{
    t_node *here_doc;

    here_doc = get_next_node_by_type(cmd, NODE_HERE_DOC);
    while (here_doc)
    {
        get_here_doc(here_doc->children);
        here_doc = get_next_node_by_type(here_doc->next, NODE_HERE_DOC);
    }
}

void collect_here_doc(t_node *root)
{
    t_node *curr;

    curr = root;
    while (curr)
    {
        collect_cmd_here_doc(curr);
        curr = curr->next;
    }
}
*/
