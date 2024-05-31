#include "interpreter.h"

void print_cmd(t_cmd cmd)
{
    if (cmd.type != CMD_SUBSHELL)
        printf("cmd argv0 = %s\n", cmd.argv[0]);
    printf("infile = %d \n", cmd.infile);
    printf("outfile = %d \n", cmd.outfile);
    printf("argc = %lu \n", cmd.argc);
    printf("argv = %p \n", cmd.argv);
    printf("binary path = %s \n", cmd.bin_path);
    printf("subshell = %p \n", cmd.subshell);
}

t_node *get_next_node_by_type(t_node *root, t_node_type type)
{
    t_node *tmp;

    tmp = root;
    while (tmp && !(tmp->type & type))
        tmp = tmp->next;
    return tmp;
}

void get_perm_flags(int *p_flags, int *m_flags, t_node_type type)
{
    if (type == NODE_REDIRECT_IN)
    {
        *p_flags = O_RDONLY;
        *m_flags = 0;
    }
    else if (type == NODE_REDIRECT_OUT)
    {
        *p_flags = O_WRONLY | O_CREAT | O_TRUNC;
        *m_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
    else if (type == NODE_APPEND)
    {
        *p_flags = O_WRONLY | O_APPEND | O_CREAT;
        *m_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
}

void close_previous_files(t_cmd *cmd, t_node_type type)
{
    if (cmd->infile != STDIN_FILENO && type == NODE_REDIRECT_IN)
    {
        close(cmd->infile);
        cmd->infile = STDIN_FILENO;
    }
    else if (cmd->outfile != STDOUT_FILENO && (type == NODE_REDIRECT_OUT || type == NODE_APPEND))
    {
        close(cmd->outfile);
        cmd->outfile = STDOUT_FILENO;
    }
}

int open_file_as(char *fname, t_cmd *cmd, t_node_type type)
{
    int fd;
    int p_flags;
    int m_flags;

    close_previous_files(cmd, type);
    get_perm_flags(&p_flags, &m_flags, type);
    fd = open(fname, p_flags, m_flags);
    if (fd < 0)
    {
        perror(fname);
        return 1;
    }
    if (type == NODE_REDIRECT_IN)
        cmd->infile = fd;
    if (type == NODE_REDIRECT_OUT || type == NODE_APPEND)
        cmd->outfile = fd;
    return 0;
}

int expand_filename_asterice(char *name, t_cmd *cmd, t_node *node)
{
    char **expanded_name;
    size_t count;
    int ret_value;

    expanded_name = malloc(2 * sizeof(char *));
    if (expanded_name == NULL)
        malloc_error(name, NULL, NULL, NULL);
    count = 1;
    expanded_name[0] = ft_strdup(name);
    expanded_name[1] = NULL;
    if (expanded_name[0] == NULL)
        malloc_error(name, NULL, NULL, NULL);
    expanded_name = expand_asterices(expanded_name, &count);
    if (expanded_name == NULL)
        malloc_error(name, NULL, NULL, NULL);
    if (count > 1)
    {
        print_error(name, "ambiguous redirection");
        free_arr(expanded_name);
        return 1;
    }
    ret_value = open_file_as(*expanded_name, cmd, node->type);
    free_arr(expanded_name);
    return ret_value;
}

int open_file_from_node(t_node *node, t_cmd *cmd)
{
    char *name;
    int ret_value;

    if (node->type == NODE_HERE_DOC)
    {
        if (cmd->infile != STDIN_FILENO)
            close(cmd->infile);
        cmd->infile = node->children->here_doc_fd;
        return 0;
    }
    name = expand_string(node->children->token_str, TRUE);
    if (name == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
    if (contains_chars(node->children->token_str, "*") == FALSE)
        ret_value = open_file_as(name, cmd, node->type);
    else
        ret_value = expand_filename_asterice(name, cmd, node);
    free(name);
    return ret_value;
}

int open_files(t_node *cmd_node, t_cmd *cmd)
{
    int ret_value;
    t_node *node;

    // if (cmd->outfile != STDOUT_FILENO)
    //     close(cmd->outfile);

    node = get_next_node_by_type(cmd_node->children, NODE_REDIRECT_IN | NODE_REDIRECT_OUT | NODE_APPEND | NODE_HERE_DOC);
    while (node)
    {
        ret_value = open_file_from_node(node, cmd);
        if (ret_value)
            return ret_value;
        node = get_next_node_by_type(node->next, NODE_REDIRECT_IN | NODE_REDIRECT_OUT | NODE_APPEND | NODE_HERE_DOC);
    }
    return 0;
}

size_t get_argc(t_node *cmd_node)
{
    size_t argc;

    argc = 0;
    cmd_node = get_next_node_by_type(cmd_node->children, NODE_STRING);
    while (cmd_node)
    {
        argc++;
        cmd_node = get_next_node_by_type(cmd_node->next, NODE_STRING);
    }
    return argc;
}

char *expand_argv(t_string str, bool *has_asterix)
{
    char *res;

    if (contains_chars(str, "*") == TRUE)
        *has_asterix = TRUE;
    res = expand_string(str, TRUE);
    return res;
}

void get_argv(t_node *cmd_node, t_cmd *cmd)
{
    t_node *tmp;
    bool has_asterix;
    size_t i;
    cmd->bin_path = NULL;
    cmd->argc = get_argc(cmd_node);
    cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
    cmd->argv[cmd->argc] = NULL;

    i = 0;
    has_asterix = FALSE;
    tmp = get_next_node_by_type(cmd_node->children, NODE_STRING);
    while (tmp)
    {
        cmd->argv[i] = expand_argv(tmp->token_str, &has_asterix);
        i++;
        tmp = get_next_node_by_type(tmp->next, NODE_STRING);
    }
    if (has_asterix == TRUE)
    {
        cmd->argv = expand_asterices(cmd->argv, &cmd->argc);
        if (cmd->argv == NULL)
            malloc_error(NULL, NULL, NULL, NULL);
    }
}

int is_builtin(const char *s)
{
    return !ft_strcmp(s, "cd") || !ft_strcmp(s, "echo") || !ft_strcmp(s, "pwd") || !ft_strcmp(s, "export") || !ft_strcmp(s, "unset") || !ft_strcmp(s, "env") || !ft_strcmp(s, "exit");
}

char *check_bin_path(t_string path, char *cmd)
{
    struct stat fstat;
    char *tmp;
    char *fullpath;

    if (path.count == 0)
    {
        fullpath = ft_strdup(cmd);
    }
    else
    {
        fullpath = ft_substr(path.s, 0, path.count);
        tmp = ft_strjoin(fullpath, "/");
        free(fullpath);
        fullpath = ft_strjoin(tmp, cmd);
        free(tmp);
    }
    if (stat(fullpath, &fstat) == 0 && fstat.st_mode & S_IFREG && fstat.st_mode & S_IXUSR)
    {
        return fullpath;
    }
    free(fullpath);
    return NULL;
}

char *get_binary_path(char *bin_name)
{
    t_string paths;
    char *raw_path;
    char *full_path;
    size_t i;

    raw_path = get_env_value("PATH");
    paths.s = raw_path;
    paths.count = 0;
    if (raw_path == NULL)
        return check_bin_path(paths, bin_name);
    i = 0;
    while (raw_path && raw_path[i])
    {
        if (raw_path[i] == ':')
        {
            full_path = check_bin_path(paths, bin_name);
            if (full_path)
                return full_path;
            paths.s = raw_path + i + 1;
            paths.count = 0;
        }
        else
            paths.count++;
        i++;
    }
    return check_bin_path(paths, bin_name);
}

int get_cmd_path(t_cmd *cmd)
{
    if (ft_strchr(cmd->argv[0], '/'))
    {
        if (access(cmd->argv[0], F_OK))
        {
            perror(cmd->argv[0]);
            return 127;
        }
        if (access(cmd->argv[0], X_OK))
        {
            perror(cmd->argv[0]);
            return 126;
        }
        cmd->bin_path = cmd->argv[0];
    }
    else
    {
        cmd->bin_path = get_binary_path(cmd->argv[0]);
        if (cmd->bin_path == NULL)
        {
            print_error(cmd->argv[0], "command not found");
            return 127;
        }
    }
    return 0;
}

int parse_cmd(t_node *node, t_cmd *cmd)
{
    int ret_value;

    if (cmd->outfile != STDOUT_FILENO)
    {
        close(cmd->outfile);
        cmd->outfile = STDOUT_FILENO;
    }
    ret_value = open_files(node, cmd);
    if (ret_value)
        return ret_value;
    cmd->type = CMD_SUBSHELL;
    cmd->subshell = get_next_node_by_type(node->children, NODE_SUBSHELL);
    if (cmd->subshell == NULL)
    {
        cmd->type = CMD_BINARY;
        get_argv(node, cmd);
        if (cmd->argc == 0)
            return 0;
        if (is_builtin(cmd->argv[0]) == FALSE)
            return get_cmd_path(cmd);
        cmd->type = CMD_BUILTIN;
    }
    return 0;
}

void queue_cmd(t_cmd *cmd, int ret_value)
{
    int pip[2];

    if (pipe(pip) == -1)
    {
        perror(cmd->argv[0]);
        exit_with_code(cmd, EXIT_FAILURE);
    }
    if (ret_value == 0)
    {
        if (cmd->outfile == STDOUT_FILENO)
            cmd->outfile = pip[1];
        else
            close(pip[1]);
        cmd->read_pipe = pip[0];
        execute_cmd(cmd, TRUE, FALSE);
        cmd->infile = pip[0];
    }
    else
    {
        close(pip[1]);
        if (cmd->infile != STDIN_FILENO)
            close(cmd->infile);
        cmd->infile = pip[0];
        free_cmd(cmd);
    }
}

int wait_on_last_piped_cmd(t_cmd *cmd, int ret_value)
{
    if (ret_value == 0)
    {
        execute_cmd(cmd, TRUE, FALSE);
        ret_value = wait_all_childs();
    }
    else
    {
        if (cmd->infile != STDIN_FILENO)
            close(cmd->infile);
        if (cmd->outfile != STDOUT_FILENO)
            close(cmd->outfile);
        free_cmd(cmd);
        wait_all_childs();
    }
    return ret_value;
}

int execute_piping(t_node **node, t_cmd *cmd)
{
    int ret_value;

    cmd->infile = STDIN_FILENO;
    cmd->outfile = STDOUT_FILENO;
    while (TRUE)
    {
        ret_value = parse_cmd(*node, cmd);
        (*node) = (*node)->next;
        if (*node == NULL || (*node)->type != NODE_PIPE)
            break;
        queue_cmd(cmd, ret_value);
        *node = (*node)->next;
    }
    return wait_on_last_piped_cmd(cmd, ret_value);
}

t_node *advance_logical_operator(t_node *operator, int ret_value)
{
    t_node *cmd_to_exec_next;

    cmd_to_exec_next = NULL;
    while (operator)
    {
        cmd_to_exec_next = operator->next;
        if (operator->type == NODE_AND && ret_value == 0)
            break;
        if (operator->type == NODE_OR && ret_value)
            break;
        operator= cmd_to_exec_next->next;
    }
    if (operator)
        return cmd_to_exec_next;
    return NULL;
}

int interpret_logical_operator(t_node **curr_node, t_node *next_operator, t_cmd *cmd)
{
    int ret_value;

    if (next_operator->type == NODE_PIPE)
    {
        ret_value = execute_piping(curr_node, cmd);
        *curr_node = advance_logical_operator(*curr_node, ret_value);
    }
    else
    {
        ret_value = parse_cmd(*curr_node, cmd);
        if (ret_value == 0)
            ret_value = execute_cmd(cmd, FALSE, TRUE);
        else
            free_cmd(cmd);
        *curr_node = advance_logical_operator(next_operator, ret_value);
    }

    return ret_value;
}

t_cmd init_cmd()
{
    t_cmd cmd;

    cmd.read_pipe = -1;
    cmd.argv = NULL;
    cmd.bin_path = NULL;
    cmd.infile = STDIN_FILENO;
    cmd.outfile = STDOUT_FILENO;
    return cmd;
}

int interpret_root(t_node *root)
{
    t_node *next_operator;
    t_cmd cmd;
    int ret_value;

    cmd = init_cmd();
    ret_value = 0;
    while (root && shell.interrupt == FALSE)
    {
        next_operator = root->next;
        if (next_operator)
            ret_value = interpret_logical_operator(&root, next_operator, &cmd);
        else
        {
            ret_value = parse_cmd(root, &cmd);
            if (ret_value == 0)
                ret_value = execute_cmd(&cmd, FALSE, TRUE);
            else
                free_cmd(&cmd);
            break;
        }
    }
    close_here_docs();
    shell.interrupt = FALSE;
    return ret_value;
}