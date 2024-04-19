#include "lexer.h"

char *get_node_type(t_node_type type)
{
    switch (type)
    {
    case NODE_CMD:
        return "NODE_CMD";
    case NODE_CMD_ARGS:
        return "NODE_CMD_ARGS";
    case NODE_STRING:
        return "NODE_STRING";
    case NODE_SQUOTE:
        return "NODE_SQUOTE";
    case NODE_DQUOTE:
        return "NODE_DQUOTE";
    case NODE_PIPE:
        return "NODE_PIPE";
    case NODE_REDIRECT_IN:
        return "NODE_REDIRECT_IN";
    case NODE_REDIRECT_OUT:
        return "NODE_REDIRECT_OUT";
    case NODE_APPEND:
        return "NODE_APPEND";
    case NODE_HERE_DOC:
        return "NODE_HERE_DOC";
    case NODE_SUBSHELL:
        return "NODE_SUBSHELL";
    default:
        assert(!"NOT POSSIBLE");
    }
    return "";
}

void print_node(t_node *node, int indent)
{
    t_node *tmp;
    printf("%*.s | Node type = %s\n", indent, "", get_node_type(node->type));
    if (node->token_str.count > 0)
        printf("%*.s | Node token raw = %.*s\n", indent, "", (int)node->token_str.count, node->token_str.s);
    printf("%*.s | Node has %ld childs\n", indent, "", node->childs_count);
    tmp = node->children;
    while (tmp)
    {
        printf("%*.s [-->\n", indent, "");
        print_node(tmp, indent + 4);
        tmp = tmp->next;
        printf("%*.s ]\n", indent, "");
    }
}

void print_tree(t_node *root)
{
    t_node *tmp;

    printf("[root contains %ld elements]\n", root->list_count);
    tmp = root;
    while (tmp)
    {
        printf("[\n");
        print_node(tmp, 0);
        printf("]\n");
        tmp = tmp->next;
    }
}

int main(int c, char **v, char **env)
{
    t_node *root;
    USED(c);
    USED(v);
    USED(env);

    init_shell();
    char *line;

    line = readline(PROMPT);
    while (line != NULL)
    {
        root = parse_line(line);

        if (root)
            print_tree(root);

        free(line);
        line = readline(PROMPT);
    }
    ft_putendl_fd("exit", 1);
    return 0;
}