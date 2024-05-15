#include "lexer.h"

t_token *parse_subshell(t_node *root, t_lexer *lexer, t_token *token)
{
    t_node *subshell;

    if (init_subshell_node(root, lexer, token, &subshell) == NULL)
        return NULL;
    while (token->type != TOKEN_EOF)
    {
        if (IS_CMD_TOKEN(*token))
        {
            if (fill_cmd(&subshell, token, lexer, TRUE) != NULL)
                continue;
            return NULL;
        }
        else if (subshell->childs_count == 0)
            return syntax_error("Millishell: empty subshell.");
        else if (IS_LOGIC_OP(*token))
        {
            if (link_logic_oper(&subshell, lexer, token, TRUE))
                continue;
            return syntax_error(SYN_ERR);
        }
        else if (token->type == TOKEN_CLOSE_PAREN)
            break;
        *token = get_next_token(lexer, TRUE);
    }
    return token;
}

int get_here_doc(t_node *node)
{
    int pip[2];
    int line;
    char *delim;

    node->here_doc_fd = -1;
    delim = ft_substr(node->token_str.s, 0, node->token_str.count);
    if (pipe(pip) == -1)
    {
        perror("pipe");
        exit(1);
    }
    char buff[1024];
    int fd = dup(0);
    line = read(fd, buff, 100);
    node->here_doc_fd = pip[0];
    while (line > 0 && shell.interrupt == FALSE)
    {
        buff[line] = 0;
        if (ft_strcmp(buff, delim) == 0)
        {
            close(pip[1]);
            // free(line);
            return 0;
        }
        ft_putendl_fd(buff, pip[1]);
        // free(line);
        line = read(fd, buff, 100);
    }
    close(fd);
    if (shell.interrupt == TRUE)
    {
        shell.interrupt = FALSE;
        return 1;
    }
    // error -> bash: warning: here-document at line 13 delimited by end-of-file (wanted `hh')
    close(pip[1]);
    syntax_error("Millishell: expacting delim for here_doc.");
    return 0;
}

int add_redirect_node(t_lexer *lexer, t_node *curr_cmd, int type)
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
        {
            syntax_error(SYN_QUOTE);
            return 1;
        }
        if (type == NODE_HERE_DOC && get_here_doc(node))
            return 1;
    }
    else
    {
        syntax_error("Millishell: expecting name of file or here_doc delim to redirect I/O.");
        return 1;
    }
    return 0;
}

t_token *fill_cmd(t_node **root, t_token *token, t_lexer *lexer, int as_child)
{
    t_node *curr_cmd;
    int can_have_subshell;

    can_have_subshell = TRUE;
    curr_cmd = link_new_node(root, NODE_CMD, as_child);
    while (IS_CMD_TOKEN(*token))
    {
        if (token->type == TOKEN_STRING && link_argv_node(curr_cmd, lexer) == NULL)
            return NULL;
        else if (IS_REDIRECT_TOKEN(*token) && add_redirect_node(lexer, curr_cmd, token->type))
            return NULL;
        else if (token->type == TOKEN_OPEN_PAREN)
        {
            if (can_have_subshell == FALSE)
                return syntax_error("Millishell: CMD ARGS BEFORE SUBSHELL OR MULTIPLE SUBSHELLS");
            if (parse_subshell(curr_cmd, lexer, token) == NULL)
                return NULL;
            if (token->type != TOKEN_CLOSE_PAREN)
                return syntax_error(SYN_BRACE);
        }
        can_have_subshell = FALSE;
        *token = get_next_token(lexer, TRUE);
    }
    return token;
}

t_node **parser_loop(t_node **root, t_lexer *lexer, t_token *token)
{
    *root = NULL;
    while (token->type != TOKEN_EOF)
    {
        if (IS_CMD_TOKEN(*token))
        {
            token = fill_cmd(root, token, lexer, FALSE);
            if (token)
                continue;
            return NULL;
        }
        if ((*root)->list_count > 0)
        {
            if (IS_LOGIC_OP(*token))
            {
                if (link_logic_oper(root, lexer, token, FALSE))
                    continue;
            }
            return syntax_error(SYN_ERR);
        }
        else
            return syntax_error(SYN_ERR);
        *token = get_next_token(lexer, TRUE);
    }
    return root;
}

t_node **parse_line(char *line, t_node **root)
{
    t_lexer lexer;
    t_token token;

    lexer = new_lexer(line);
    token = get_next_token(&lexer, TRUE);
    if (token.type == TOKEN_EOF)
        return NULL;
    if (!IS_CMD_TOKEN(token))
        return syntax_error(SYN_ERR);
    return parser_loop(root, &lexer, &token);
}
