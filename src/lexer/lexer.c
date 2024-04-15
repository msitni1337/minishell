#include "lexer.h"

t_string get_string_whitespace(t_lexer *lexer)
{
    t_string res;
    int escape;

    res.s = &(lexer->line[lexer->pos - 1]);
    res.count = 1;
    escape = FALSE;
    while (lexer->pos < lexer->count)
    {
        if (!escape && ft_isspace(lexer->line[lexer->pos]))
            break;
        if (lexer->line[lexer->pos] == '\\')
            escape = !escape;
        else
            escape = FALSE;
        res.count++;
        lexer->pos++;
    }
    return res;
}

t_string get_string_delim(t_lexer *lexer, const char delim)
{
    t_string res;
    int escape;

    res.s = &(lexer->line[lexer->pos]);
    res.count = 0;
    escape = FALSE;
    while (lexer->pos < lexer->count)
    {
        if (!escape && lexer->line[lexer->pos] == delim)
            break;
        if (lexer->line[lexer->pos] == '\\')
            escape = !escape;
        else
            escape = FALSE;
        res.count++;
        lexer->pos++;
    }
    if (lexer->line[lexer->pos] == delim)
    {
        lexer->pos++;
        return res;
    }
    return res; // todo => need to throw error "syntax error"
}

t_node *get_node_by_type(t_node *root, t_node_type type, int create_new)
{
    t_node *tmp;

    tmp = root;
    if (!tmp)
        return NULL;
    tmp = root->children;
    while (tmp && tmp->type != type)
        tmp = tmp->next;
    if (!tmp && create_new)
    {
        tmp = create_node(type);
        append_node(&(root->children), tmp, &(root->childs_count));
    }
    return tmp;
}

t_node * parse_line(char *line)
{
    t_lexer lexer;
    t_node *root;
    t_node *curr_cmd;
    t_token token;

    lexer = new_lexer(line);
    token = get_next_token(&lexer, TRUE);
    // To crash program if invalid token.. just fot testing purposes
    assert(token.type != TOKEN_INVALID);
    if (token.type == TOKEN_PIPE || token.type == TOKEN_AND || token.type == TOKEN_OR)
        assert(!"Throw syntax error");
    // roots = init_da(sizeof(t_node *), create_node(NODE_CMD));
    root = create_node(NODE_CMD);
    root->list_count++;
    curr_cmd = root;
    while (token.type != TOKEN_EOF)
    {
        if (token.type == TOKEN_DQUOTE)
        {
            t_node *cmd_args_node = get_node_by_type(curr_cmd, NODE_CMD_ARGS, TRUE);
            add_dquote_node(&cmd_args_node, &lexer, TRUE);
        }
        else if (token.type == TOKEN_SQUOTE)
        {
            t_node *cmd_args_node = get_node_by_type(curr_cmd, NODE_CMD_ARGS, TRUE);
            add_squote_node(&cmd_args_node, &lexer, TRUE);
        }
        else if (token.type == TOKEN_STRING)
        {
            t_node *cmd_args_node = get_node_by_type(curr_cmd, NODE_CMD_ARGS, TRUE);
            add_str_node(&cmd_args_node, &lexer, TRUE);
        }
        else if (token.type == TOKEN_REDIRECT_IN)
        {
            t_node *cmd_red_in_node = get_node_by_type(curr_cmd, NODE_REDIRECT_IN, TRUE);
            token = get_next_token(&lexer, TRUE);
            if (token.type == TOKEN_STRING || token.type == TOKEN_DQUOTE || token.type == TOKEN_SQUOTE)
            {
                t_node *node = create_node(NODE_STRING);
                if (token.type == TOKEN_SQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, SQUOTE);
                    node->type = NODE_SQUOTE;
                }
                else if (token.type == TOKEN_DQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, DQUOTE);
                    node->type = NODE_DQUOTE;
                }
                else
                    node->token_str = get_string_whitespace(&lexer);
                append_node(&(cmd_red_in_node->children), node, &(cmd_red_in_node->childs_count));
            }
            else
                assert(!"Throw syntax error");
        }
        else if (token.type == TOKEN_REDIRECT_OUT)
        {
            t_node *cmd_red_in_node = get_node_by_type(curr_cmd, NODE_REDIRECT_OUT, TRUE);
            token = get_next_token(&lexer, TRUE);
            if (token.type == TOKEN_STRING || token.type == TOKEN_DQUOTE || token.type == TOKEN_SQUOTE)
            {
                t_node *node = create_node(NODE_STRING);
                if (token.type == TOKEN_SQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, SQUOTE);
                    node->type = NODE_SQUOTE;
                }
                else if (token.type == TOKEN_DQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, DQUOTE);
                    node->type = NODE_DQUOTE;
                }
                else
                    node->token_str = get_string_whitespace(&lexer);
                append_node(&(cmd_red_in_node->children), node, &(cmd_red_in_node->childs_count));
            }
            else
                assert(!"Throw syntax error");
        }
        else if (token.type == TOKEN_HERE_DOC)
        {
            t_node *cmd_red_in_node = get_node_by_type(curr_cmd, NODE_HERE_DOC, TRUE);
            token = get_next_token(&lexer, TRUE);
            if (token.type == TOKEN_STRING || token.type == TOKEN_DQUOTE || token.type == TOKEN_SQUOTE)
            {
                t_node *node = create_node(NODE_STRING);
                if (token.type == TOKEN_SQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, SQUOTE);
                    node->type = NODE_SQUOTE;
                }
                else if (token.type == TOKEN_DQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, DQUOTE);
                    node->type = NODE_DQUOTE;
                }
                else
                    node->token_str = get_string_whitespace(&lexer);
                append_node(&(cmd_red_in_node->children), node, &(cmd_red_in_node->childs_count));
            }
            else
                assert(!"Throw syntax error");
        }
        else if (token.type == TOKEN_APPEND)
        {
            t_node *cmd_red_in_node = get_node_by_type(curr_cmd, NODE_APPEND, TRUE);
            token = get_next_token(&lexer, TRUE);
            if (token.type == TOKEN_STRING || token.type == TOKEN_DQUOTE || token.type == TOKEN_SQUOTE)
            {
                t_node *node = create_node(NODE_STRING);
                if (token.type == TOKEN_SQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, SQUOTE);
                    node->type = NODE_SQUOTE;
                }
                else if (token.type == TOKEN_DQUOTE)
                {
                    node->token_str = get_string_delim(&lexer, DQUOTE);
                    node->type = NODE_DQUOTE;
                }
                else
                    node->token_str = get_string_whitespace(&lexer);
                append_node(&(cmd_red_in_node->children), node, &(cmd_red_in_node->childs_count));
            }
            else
                assert(!"Throw syntax error");
        }
        else if (token.type == TOKEN_PIPE)
        {
            t_node*node = create_node(NODE_PIPE);
            append_node(&root, node, &(root->list_count));
        }
        
        token = get_next_token(&lexer, TRUE);
        assert(token.type != TOKEN_INVALID);
    }
    return root;
}
