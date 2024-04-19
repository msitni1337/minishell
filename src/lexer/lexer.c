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
        if (!escape && (ft_isspace(lexer->line[lexer->pos]) || is_special(lexer->line[lexer->pos])))
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
    assert(!"NO END QUOTE PROVIDED");
    return res; // todo => need to throw error "syntax error"
}

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

void add_redirect_node(t_lexer *lexer, t_node *curr_cmd, t_node_type type)
{
    t_token token;
    t_node *red_node;

    red_node = create_node(type);
    append_child(curr_cmd, red_node);
    token = get_next_token(lexer, TRUE);
    if (token.type == TOKEN_SQUOTE)
        add_squote_node(red_node, lexer);
    else if (token.type == TOKEN_DQUOTE)
        add_dquote_node(red_node, lexer);
    else if (token.type == TOKEN_STRING)
        add_str_node(red_node, lexer);
    else
        assert(!"Throw syntax error");
}

t_token fill_cmd(t_node **root, t_token token, t_lexer *lexer, int as_child)
{
    t_node *curr_cmd;

    curr_cmd = create_node(NODE_CMD);
    if (as_child)
        append_child(*root, curr_cmd);
    else
        append_node(root, curr_cmd);
    while (IS_CMD_TOKEN(token))
    {
        if (token.type == TOKEN_DQUOTE)
            add_dquote_node(curr_cmd, lexer);
        else if (token.type == TOKEN_SQUOTE)
            add_squote_node(curr_cmd, lexer);
        else if (token.type == TOKEN_STRING)
            add_str_node(curr_cmd, lexer);
        else if (token.type == TOKEN_REDIRECT_IN)
            add_redirect_node(lexer, curr_cmd, NODE_REDIRECT_IN);
        else if (token.type == TOKEN_REDIRECT_OUT)
            add_redirect_node(lexer, curr_cmd, NODE_REDIRECT_OUT);
        else if (token.type == TOKEN_HERE_DOC)
            add_redirect_node(lexer, curr_cmd, NODE_HERE_DOC);
        else if (token.type == TOKEN_APPEND)
            add_redirect_node(lexer, curr_cmd, NODE_APPEND);
        token = get_next_token(lexer, TRUE);
        assert(token.type != TOKEN_INVALID);
    }
    return token;
}

t_token parse_subshell(t_node **root, t_lexer *lexer)
{
    t_node *subshell;
    t_token token;

    subshell = create_node(NODE_SUBSHELL);
    append_node(root, subshell);
    token = get_next_token(lexer, TRUE);
    if (token.type == TOKEN_EOF)
        return token;
    // roots = init_da(sizeof(t_node *), create_node(NODE_CMD));
    //root = NULL;
    while (token.type != TOKEN_EOF)
    {
        if (IS_CMD_TOKEN(token))
        {
            token = fill_cmd(&subshell, token, lexer, TRUE);
            continue;
        }
        else if (token.type == TOKEN_PIPE)
        {
            t_node *node = create_node(NODE_PIPE);
            append_child(subshell, node);
            token = get_next_token(lexer, TRUE);
            if (!IS_CMD_TOKEN(token) && token.type != TOKEN_OPEN_PAREN)
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_AND)
        {
            t_node *node = create_node(NODE_AND);
            append_child(subshell, node);
            token = get_next_token(lexer, TRUE);
            if (!IS_CMD_TOKEN(token) && token.type != TOKEN_OPEN_PAREN)
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_OR)
        {
            t_node *node = create_node(NODE_OR);
            append_child(subshell, node);
            token = get_next_token(lexer, TRUE);
            if (!IS_CMD_TOKEN(token) && token.type != TOKEN_OPEN_PAREN)
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_OPEN_PAREN)
        {
            token = parse_subshell(&(subshell->children), lexer);
            assert(token.type == TOKEN_CLOSE_PAREN);
        }
        else if (token.type == TOKEN_CLOSE_PAREN)
            return token;
        token = get_next_token(lexer, TRUE);
        assert(token.type != TOKEN_INVALID);
    }
    return token;
}

t_node *parse_line(char *line)
{
    t_lexer lexer;
    t_node *root;
    t_token token;

    lexer = new_lexer(line);
    token = get_next_token(&lexer, TRUE);
    // To crash program if invalid token.. just fot testing purposes
    assert(IS_CMD_TOKEN(token) || token.type == TOKEN_OPEN_PAREN);
    if (token.type == TOKEN_EOF)
        return NULL;
    // roots = init_da(sizeof(t_node *), create_node(NODE_CMD));
    root = NULL;
    while (token.type != TOKEN_EOF)
    {
        if (IS_CMD_TOKEN(token))
        {
            token = fill_cmd(&root, token, &lexer, FALSE);
            continue;
        }
        else if (token.type == TOKEN_PIPE)
        {
            t_node *node = create_node(NODE_PIPE);
            append_node(&root, node);
            token = get_next_token(&lexer, TRUE);
            if (!IS_CMD_TOKEN(token) && token.type != TOKEN_OPEN_PAREN)
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_AND)
        {
            t_node *node = create_node(NODE_AND);
            append_node(&root, node);
            token = get_next_token(&lexer, TRUE);
            if (!IS_CMD_TOKEN(token) && token.type != TOKEN_OPEN_PAREN)
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_OR)
        {
            t_node *node = create_node(NODE_OR);
            append_node(&root, node);
            token = get_next_token(&lexer, TRUE);
            if (!IS_CMD_TOKEN(token) && token.type != TOKEN_OPEN_PAREN)
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_OPEN_PAREN)
        {
            token = parse_subshell(&root, &lexer);
            assert(token.type == TOKEN_CLOSE_PAREN);
        }
        else if (token.type == TOKEN_CLOSE_PAREN)
            assert(!"THROW SYNTAX ERROR");
        token = get_next_token(&lexer, TRUE);
        assert(token.type != TOKEN_INVALID);
    }
    return root;
}
