#include "lexer.h"

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

t_token parse_subshell(t_node *root, t_lexer *lexer)
{
    t_node *subshell;
    t_token token;

    subshell = create_node(NODE_SUBSHELL);
    append_child(root, subshell);
    token = get_next_token(lexer, TRUE);
    if (token.type == TOKEN_EOF)
        assert(!"SYNTAX ERROR"); // syntax error unclosed paren
    // roots = init_da(sizeof(t_node *), create_node(NODE_CMD));
    // root = NULL;
    while (token.type != TOKEN_EOF)
    {
        if (IS_CMD_TOKEN(token))
        {
            token = fill_cmd(&subshell, token, lexer, TRUE);
            continue;
        }
        else if (subshell->childs_count == 0)
            assert(!"THROW SYNTAX ERROR\n");
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
        // else if (token.type == TOKEN_OPEN_PAREN)
        // {
        //     token = parse_subshell(&(subshell->children), lexer);
        //     assert(token.type == TOKEN_CLOSE_PAREN);
        // }
        else if (token.type == TOKEN_CLOSE_PAREN)
            return token;
        token = get_next_token(lexer, TRUE);
        assert(token.type != TOKEN_INVALID);
    }
    assert(token.type == TOKEN_CLOSE_PAREN); // syntax error unclosed paren
    return token;
}

void get_here_doc(t_node *node)
{
    int pip[2];
    char *line;
    char *delim;

    node->here_doc_fd = -1;
    delim = ft_substr(node->token_str.s, 0, node->token_str.count);
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

void add_redirect_node(t_lexer *lexer, t_node *curr_cmd, t_node_type type)
{
    t_token token;
    t_node *red_node;

    red_node = create_node(type);
    append_child(curr_cmd, red_node);
    token = get_next_token(lexer, TRUE);
    if (token.type == TOKEN_STRING)
    {
        t_node *node = add_str_node(red_node, lexer);
        if (node == NULL)
            assert(!"throw syntax error");
        if (type == NODE_HERE_DOC)
        {
            get_here_doc(node);
        }
    }
    /*
else if (type == NODE_REDIRECT_IN && token.type == TOKEN_OPEN_PAREN
    && lexer->line[lexer->pos - 2] == '<')
    parse_subshell(red_node, lexer);
    */
    else
        assert(!"Throw syntax error");
}

t_token fill_cmd(t_node **root, t_token token, t_lexer *lexer, int as_child)
{
    t_node *curr_cmd;
    int can_have_subshell;

    can_have_subshell = TRUE;
    curr_cmd = create_node(NODE_CMD);
    if (as_child)
        append_child(*root, curr_cmd);
    else
        append_node(root, curr_cmd);
    while (IS_CMD_TOKEN(token))
    {
        if (token.type == TOKEN_STRING)
        {
            if (add_str_node(curr_cmd, lexer) == NULL)
                assert(!"THROW SYNTAX ERROR"); // Error Unclosed Quote..
            if (get_node_by_type(curr_cmd, NODE_SUBSHELL))
                assert(!"THROW SYNTAX ERROR"); // bash: syntax error near unexpected token `cat' -> (ls) cat
        }
        else if (token.type == TOKEN_REDIRECT_IN)
            add_redirect_node(lexer, curr_cmd, NODE_REDIRECT_IN);
        else if (token.type == TOKEN_REDIRECT_OUT)
            add_redirect_node(lexer, curr_cmd, NODE_REDIRECT_OUT);
        else if (token.type == TOKEN_HERE_DOC)
            add_redirect_node(lexer, curr_cmd, NODE_HERE_DOC);
        else if (token.type == TOKEN_APPEND)
            add_redirect_node(lexer, curr_cmd, NODE_APPEND);
        else if (token.type == TOKEN_OPEN_PAREN)
        {
            if (can_have_subshell == TRUE)
                parse_subshell(curr_cmd, lexer);
            else
                assert(!"THROW SYNTAX ERROR"); // bash: syntax error near unexpected token `ls' -> cat (ls)
        }
        can_have_subshell = FALSE;
        /*
        else if (token.type == TOKEN_SUBSHELL_ARG)
            parse_subshell(curr_cmd, lexer, NODE_SUBSHELL_ARG);
        */
        token = get_next_token(lexer, TRUE);
        assert(token.type != TOKEN_INVALID);
    }
    if (curr_cmd->childs_count == 0)
        assert(!"THROW SYNTAX ERROR\n");
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
    if (token.type == TOKEN_EOF)
        return NULL;
    assert(IS_CMD_TOKEN(token) || token.type == TOKEN_OPEN_PAREN);
    // roots = init_da(sizeof(t_node *), create_node(NODE_CMD));
    root = NULL;
    while (token.type != TOKEN_EOF)
    {
        if (IS_CMD_TOKEN(token))
            token = fill_cmd(&root, token, &lexer, FALSE);
        else if (root->list_count == 0)
            assert(!"THROW SYNTAX ERROR\n");
        if (token.type == TOKEN_PIPE)
        {
            t_node *node = create_node(NODE_PIPE);
            append_node(&root, node);
            token = get_next_token(&lexer, TRUE);
            if (!IS_CMD_TOKEN(token))
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_AND)
        {
            t_node *node = create_node(NODE_AND);
            append_node(&root, node);
            token = get_next_token(&lexer, TRUE);
            if (!IS_CMD_TOKEN(token))
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_OR)
        {
            t_node *node = create_node(NODE_OR);
            append_node(&root, node);
            token = get_next_token(&lexer, TRUE);
            if (!IS_CMD_TOKEN(token))
                assert(!"THROW SYNTAX ERROR\n");
            continue;
        }
        else if (token.type == TOKEN_CLOSE_PAREN)
            assert(!"THROW SYNTAX ERROR");
        token = get_next_token(&lexer, TRUE);
        assert(token.type != TOKEN_INVALID);
    }
    return root;
}
