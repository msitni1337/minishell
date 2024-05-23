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
            return syntax_error("empty subshell.");
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

int write_next_line_here_doc(t_node *node, int write)
{
    char *line;
    char *delim;

    shell.collecting_here_doc = TRUE;
    delim = ft_substr(node->token_str.s, 0, node->token_str.count);
    line = readline("> ");
    while (line)
    {
        if (ft_strcmp(line, delim) == 0)
            break;
        ft_putendl_fd(line, write);
        free(line);
        line = readline("> ");
    }
    close(write);
    free(delim);
    if (line)
    {
        free(line);
        return 0;
    }
    return 1;
}

int get_here_doc(t_node *node)
{
    int pip[2];
    int stdin_dup;

    stdin_dup = dup(0);
    if (pipe(pip) == -1)
    {
        perror("pipe");
        exit(1);
    }
    node->here_doc_fd = pip[0];
    if (write_next_line_here_doc(node, pip[1]))
    {
        if (shell.interrupt == TRUE)
        {
            shell.collecting_here_doc = FALSE;
            dup2(stdin_dup, 0);
            close(stdin_dup);
            return 1;
        }
        syntax_error("expecting delim for here_doc.");
    }
    close(stdin_dup);
    shell.collecting_here_doc = FALSE;
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
        syntax_error("expecting name of file or here_doc delim to redirect I/O.");
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
                return syntax_error("CMD ARGS BEFORE SUBSHELL OR MULTIPLE SUBSHELLS");
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
    shell.collecting_here_doc = FALSE;
    return root;
}

char *get_next_asterix(char *line)
{
}

char *expand_asterices(char *line)
{
    if (ft_strchr(line, '*'))
    {
        t_lexer lexer;
        t_token token;
        char *asterix;
        char *tmp;
        char *res;
        size_t i;
        size_t copied;

        res = NULL;
        lexer = new_lexer(line);
        token = get_next_token(&lexer, TRUE);
        if (lexer.pos)
        {
            tmp = ft_substr(lexer.line, 0, lexer.pos);
            res = ft_strjoin(res, tmp);
            free(tmp);
            lexer = new_lexer(line + lexer.pos);
        }
        while (token.type != TOKEN_EOF)
        {
            if (token.type == TOKEN_STRING)
            {
                t_string string;
                get_string(&lexer, &string);
                lexer = new_lexer(line + lexer.pos);
                i = 0;
                tmp = NULL;
                while (i < string.count)
                {
                    if (*(string.s + i) == '*')
                    {
                        tmp = expand_asterice(string);
                        break;
                    }
                    i++;
                }
                if (tmp == NULL)
                    tmp = ft_substr(node.token_str.s, 0, node.token_str.count);
                ft_strjoin(res, tmp);
                free(tmp);
            }
            token = get_next_token(&lexer, TRUE);
            if (lexer.pos)
            {
                tmp = ft_substr(lexer.line, 0, lexer.pos);
                res = ft_strjoin(res, tmp);
                free(tmp);
                lexer = new_lexer(line + lexer.pos);
            }
        }
        return res;
    }
    return line;
}

t_node **parse_line(char *line, t_node **root)
{
    t_lexer lexer;
    t_token token;

    //  line = expand_asterices(line);
    lexer = new_lexer(line);
    token = get_next_token(&lexer, TRUE);
    if (token.type == TOKEN_EOF)
        return NULL;
    if (!IS_CMD_TOKEN(token))
        return syntax_error(SYN_ERR);
    return parser_loop(root, &lexer, &token);
}
