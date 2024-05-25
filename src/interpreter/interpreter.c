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

int open_file_as(char *fname, t_cmd *cmd, t_node_type type)
{
    int fd;
    int p_flags;
    int m_flags;

    if (type == NODE_REDIRECT_IN)
    {
        if (cmd->infile != STDIN_FILENO)
            close(cmd->infile);
        p_flags = O_RDONLY;
        m_flags = 0;
    }
    if (type == NODE_HERE_DOC)
    {
        if (cmd->infile != STDIN_FILENO)
            close(cmd->infile);
        return 0;
    }

    if (type == NODE_REDIRECT_OUT)
    {
        if (cmd->outfile != STDOUT_FILENO)
            close(cmd->outfile);
        p_flags = O_WRONLY | O_CREAT | O_TRUNC;
        m_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }

    if (type == NODE_APPEND)
    {
        if (cmd->outfile != STDOUT_FILENO)
            close(cmd->outfile);
        p_flags = O_WRONLY | O_APPEND | O_CREAT;
        m_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }

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

int open_files(t_node *cmd_node, t_cmd *cmd)
{
    int ret_value;
    t_node *node;

    // if (cmd->outfile != STDOUT_FILENO)
    //     close(cmd->outfile);

    node = get_next_node_by_type(cmd_node->children, NODE_REDIRECT_IN | NODE_REDIRECT_OUT | NODE_APPEND | NODE_HERE_DOC);
    while (node)
    {
        if (contains_chars(node->children->token_str, "*") == TRUE) 
        {
            assert(!"NOT IMPLEMENTD");
            return 1;
        }
        char *name = expand_string(node->children->token_str, TRUE);
        ret_value = open_file_as(name, cmd, node->type);
        if (ret_value)
            return ret_value;
        if (node->type == NODE_HERE_DOC)
            cmd->infile = node->children->here_doc_fd;
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

    if (contains_chars(str, "*") == FALSE)
    {
        res = expand_string(str, TRUE);
    }
    else
    {
        res = expand_string(str, FALSE);
        *has_asterix = TRUE;
    }
    return res;
}

void get_argv(t_node *cmd_node, t_cmd *cmd)
{
    t_node *tmp;
    bool has_asterix;
    size_t i;
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
        cmd->argv = expand_asterices_argv(cmd->argv, &cmd->argc);
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

char *get_binary_path(char *cmd)
{
    t_string tmp;
    char *full_path;
    char *raw_path;
    int i;

    raw_path = get_env_value("PATH");
    i = 0;
    tmp.s = raw_path;
    tmp.count = 0;
    while (raw_path)
    {
        if (raw_path[i] == ':')
        {
            full_path = check_bin_path(tmp, cmd);
            if (full_path)
                return full_path;
            i++;
            tmp.s = raw_path + i;
            tmp.count = 0;
            continue;
        }
        if (raw_path[i] == 0)
        {
            full_path = check_bin_path(tmp, cmd);
            if (full_path)
                return full_path;
            break;
        }
        tmp.count++;
        i++;
    }
    return NULL;
}

int get_cmd_path(t_cmd *cmd)
{
    char *argv0;

    argv0 = cmd->argv[0];
    if (ft_strchr(argv0, '/'))
    {
        if (access(argv0, F_OK))
        {
            perror("minishell");
            return 127;
        }
        if (access(argv0, X_OK))
        {
            perror("minishell");
            return 126;
        }
        cmd->bin_path = argv0;
    }
    else
    {
        cmd->bin_path = get_binary_path(argv0);
        if (!cmd->bin_path)
        {
            /*TODO LOG ERROR  "command not found" */
            perror("command not found");
            return 127;
        }
    }
    return 0;
}

int parse_cmd(t_node *node, t_cmd *cmd)
{
    int ret_value;
    t_node *subshell;

    // if (cmd->read_pipe != -1)
    // {
    //     cmd->infile = dup(cmd->read_pipe);
    //     close(cmd->read_pipe);
    //     cmd->read_pipe = -1;
    // }
    if (cmd->outfile != STDOUT_FILENO)
    {
        close(cmd->outfile);
        cmd->outfile = STDOUT_FILENO;
    }
    ret_value = open_files(node, cmd);
    if (ret_value)
        return ret_value;
    subshell = get_next_node_by_type(node->children, NODE_SUBSHELL);
    if (subshell)
    {
        cmd->type = CMD_SUBSHELL;
        cmd->subshell = subshell;
    }
    else
    {
        cmd->type = CMD_BINARY;
        get_argv(node, cmd);
        if (cmd->argc > 0)
        {
            if (is_builtin(cmd->argv[0]))
                cmd->type = CMD_BUILTIN;
            else
                return get_cmd_path(cmd);
        }
    }
    return 0;
}

int execute_piping(t_node **node, t_cmd *cmd)
{
    int pip[2];
    int ret_value;

    cmd->infile = STDIN_FILENO;
    cmd->outfile = STDOUT_FILENO;
    while (TRUE)
    {
        ret_value = parse_cmd(*node, cmd);
        assert(cmd->infile != -1 && cmd->outfile != -1);
        (*node) = (*node)->next;
        if (*node == NULL || (*node)->type != NODE_PIPE)
            break;
        // if (cmd->outfile != STDOUT_FILENO)
        //     close(cmd->outfile);
        if (pipe(pip) == -1)
        {
            perror(cmd->argv[0]);
            exit(errno);
        }
        if (ret_value == 0)
        {
            if (cmd->outfile == STDOUT_FILENO)
                cmd->outfile = pip[1];
            else
                close(pip[1]);
            cmd->read_pipe = pip[0];
            execute_cmd(*cmd, TRUE, FALSE);
            cmd->infile = pip[0];
        }
        else
        {
            close(pip[1]);
            if (cmd->infile != STDIN_FILENO)
                close(cmd->infile);
            cmd->infile = pip[0];
        }
        (*node) = (*node)->next;
        assert(*node != NULL);
    }
    if (ret_value == 0)
    {
        execute_cmd(*cmd, TRUE, FALSE);
        ret_value = wait_all_childs();
    }
    else
    {
        if (cmd->infile != STDIN_FILENO)
            close(cmd->infile);
        if (cmd->outfile != STDOUT_FILENO)
            close(cmd->outfile);
        wait_all_childs();
    }
    return ret_value;
}

t_node *advance_logical_operator(t_node *operator, int ret_value)
{
    t_node *cmd_to_exec;

    cmd_to_exec = NULL;
    while (operator)
    {
        cmd_to_exec = operator->next;
        if (operator->type == NODE_AND && ret_value == 0)
            break;
        if (operator->type == NODE_OR && ret_value)
            break;
        operator= cmd_to_exec->next;
    }
    if (operator)
        return cmd_to_exec;
    return NULL;
}

int interpret_root(t_node *root)
{
    int ret_value;
    t_cmd cmd;
    t_node *tmp;
    t_node *node;

    node = root;
    cmd.read_pipe = -1;
    ret_value = 0;
    while (node && shell.interrupt == FALSE)
    {
        cmd.infile = STDIN_FILENO;
        cmd.outfile = STDOUT_FILENO;
        tmp = node->next;
        if (tmp)
        {
            if (tmp->type == NODE_PIPE)
            {
                ret_value = execute_piping(&node, &cmd);
                node = advance_logical_operator(node, ret_value);
            }
            else
            {
                ret_value = parse_cmd(node, &cmd);
                if (ret_value == 0)
                    ret_value = execute_cmd(cmd, FALSE, TRUE);
                node = advance_logical_operator(tmp, ret_value);
            }
        }
        else
        {
            ret_value = parse_cmd(node, &cmd);
            if (ret_value == 0)
                ret_value = execute_cmd(cmd, FALSE, TRUE);
            break;
        }
    }
    if (cmd.infile != STDIN_FILENO)
        close(cmd.infile);
    if (cmd.outfile != STDOUT_FILENO)
        close(cmd.outfile);
    if (shell.interrupt == TRUE)
    {
        shell.interrupt = FALSE;
        assert(ret_value == 130);
        return ret_value; // 130 ??
    }
    return ret_value;
}