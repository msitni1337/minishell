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
    if (delim == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
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

int randomize_str(char *str, int rand_fd)
{
    unsigned char c;
    size_t len;

    len = ft_strlen(str);
    if (read(rand_fd, str, len) == -1)
    {
        perror("read");
        exit_with_code(NULL, errno);
    }
    while (str && *str)
    {
        c = *str;
        c = (c % 10) + '0';
        *str = c;
        str++;
    }
    return 1;
}

void get_here_doc_filename(char *filename)
{
    int rand_fd;
    int tries;

    rand_fd = open("/dev/random", O_RDONLY);
    if (rand_fd == -1)
    {
        perror("open");
        free(filename);
        exit_with_code(NULL, errno);
    }
    tries = randomize_str(filename + 25, rand_fd);
    while (tries < 100 && access(filename, F_OK) == 0)
        tries += randomize_str(filename + 25, rand_fd);
    if (access(filename, F_OK) == 0)
    {
        ft_putstr_fd(HERE_DOC_FILENAME_ERROR, STDERR_FILENO);
        free(filename);
        exit_with_code(NULL, EXIT_FAILURE);
    }
    close(rand_fd);
}

void init_here_doc(int *fd, int *stdin_dup)
{
    char *filename;

    filename = ft_strdup("/tmp/" PROG_NAME "_here_doc_XXXXXXXX");
    if (filename == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
    get_here_doc_filename(filename);
    *stdin_dup = dup(0);
    if (*stdin_dup == -1)
    {
        perror("dup");
        free(filename);
        exit_with_code(NULL, errno);
    }
    *fd = open(filename, O_RDWR | O_CREAT);
    if (*fd == -1)
    {
        perror("open");
        free(filename);
        exit_with_code(NULL, errno);
    }
    free(filename);
}

int get_here_doc(t_node *node)
{
    int fd;
    int stdin_dup;

    init_here_doc(&fd, &stdin_dup);
    node->here_doc_fd = fd;
    if (write_next_line_here_doc(node, fd))
    {
        if (shell.interrupt == TRUE)
        {
            shell.collecting_here_doc = FALSE;
            if (dup2(stdin_dup, 0) == -1)
            {
                perror("dup2");
                exit_with_code(NULL, errno);
            }
            close(stdin_dup);
            close(fd);
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
    t_node *tmp;

    tmp = create_node(type);
    if (tmp == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
    append_child(curr_cmd, tmp);
    token = get_next_token(lexer, TRUE);
    if (token.type == TOKEN_STRING)
    {
        tmp = add_str_node(tmp, lexer);
        if (tmp == NULL)
        {
            syntax_error(SYN_QUOTE);
            return 1;
        }
        if (type == NODE_HERE_DOC && get_here_doc(tmp))
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
