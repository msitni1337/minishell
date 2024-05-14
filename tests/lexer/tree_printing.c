#include "lexer.h"

char *get_token_type(t_token_type type)
{
    switch (type)
    {
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_STRING:
        return "TOKEN_STRING";
    case TOKEN_PIPE:
        return "TOKEN_PIPE";
    case TOKEN_REDIRECT_IN:
        return "TOKEN_REDIRECT_IN";
    case TOKEN_REDIRECT_OUT:
        return "TOKEN_REDIRECT_OUT";
    case TOKEN_APPEND:
        return "TOKEN_APPEND";
    case TOKEN_HERE_DOC:
        return "TOKEN_HERE_DOC";
    case TOKEN_AND:
        return "TOKEN_AND";
    case TOKEN_OR:
        return "TOKEN_OR";
    case TOKEN_OPEN_PAREN:
        return "TOKEN_OPEN_PAREN";
    case TOKEN_CLOSE_PAREN:
        return "TOKEN_CLOSE_PAREN";
    default:
        assert(!"NOT POSSIBLE");
        break;
    }
    return "";
}

char *get_node_type(t_node_type type)
{
    switch (type)
    {
    case NODE_CMD:
        return "NODE_CMD";
    case NODE_STRING:
        return "NODE_STRING";
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
    case NODE_AND:
        return "NODE_AND";
    case NODE_OR:
        return "NODE_OR";
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