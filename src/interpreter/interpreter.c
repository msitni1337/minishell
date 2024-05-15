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
        return errno;
    }
    if (type == NODE_REDIRECT_IN)
        cmd->infile = fd;
    if (type == NODE_REDIRECT_OUT || type == NODE_APPEND)
        cmd->outfile = fd;
    return 0;
}

// todo return all fds.
int open_files(t_node *cmd_node, t_cmd *cmd)
{
    int ret_value;
    t_node *node;

    if (cmd->outfile != STDOUT_FILENO)
        close(cmd->outfile);
    cmd->outfile = STDOUT_FILENO;

    // todo Protect cmd->children
    node = get_next_node_by_type(cmd_node->children, NODE_REDIRECT_IN | NODE_REDIRECT_OUT | NODE_APPEND | NODE_HERE_DOC);
    while (node)
    {
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

void get_argv(t_node *cmd_node, t_cmd *cmd)
{
    t_node *tmp;
    size_t i;
    cmd->argc = get_argc(cmd_node);
    cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
    cmd->argv[cmd->argc] = NULL;

    i = 0;
    tmp = get_next_node_by_type(cmd_node->children, NODE_STRING);
    while (tmp)
    {
        cmd->argv[i] = expand_string(tmp->token_str, TRUE);
        i++;
        tmp = get_next_node_by_type(tmp->next, NODE_STRING);
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
        if (access(argv0, X_OK))
        {
            perror("minishell");
            return errno;
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

    if (cmd->read_pipe != -1)
    {
        cmd->infile = dup(cmd->read_pipe);
        close(cmd->read_pipe);
        cmd->read_pipe = -1;
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

int interpret_root(t_node *root)
{
    int ret_value;
    int pip[2];
    t_cmd cmd;
    t_node *node;

    node = root;
    shell.interrupt = FALSE;
    shell.childs_pids.count = 0;
    cmd.infile = STDIN_FILENO;
    cmd.outfile = STDOUT_FILENO;
    cmd.read_pipe = -1;
    ret_value = 0;
    while (node && shell.interrupt == FALSE)
    {
        ret_value = parse_cmd(node, &cmd);
        assert(cmd.infile != -1 && cmd.outfile != -1);
        node = node->next;
        if (node)
        {
            if (node->type == NODE_PIPE)
            {
                if (pipe(pip) == -1)
                {
                    perror(cmd.argv[0]);
                    exit(errno);
                }
                if (!ret_value)
                {
                    if (cmd.outfile != STDOUT_FILENO)
                        close(cmd.outfile);
                    cmd.outfile = pip[1];
                    cmd.read_pipe = pip[0];
                    execute_cmd(cmd, FALSE);
                    // close(pip[1]);
                }
                else
                {
                    close(pip[1]);
                    cmd.read_pipe = pip[0];
                }
            }
            else if (node->type == NODE_AND)
            {
                if (ret_value)
                    break;
                else
                {
                    ret_value = execute_cmd(cmd, TRUE);
                    if (ret_value)
                        break;
                    cmd.infile = 0;
                    cmd.outfile = 1;
                }
            }
            else if (node->type == NODE_OR && !ret_value)
            {
                ret_value = execute_cmd(cmd, TRUE);
                if (!ret_value)
                    break;
                cmd.infile = 0;
                cmd.outfile = 1;
            }
            node = node->next;
        }
        else if (!ret_value)
        {
            ret_value = execute_cmd(cmd, TRUE);
        }
    }
    if (cmd.infile != STDIN_FILENO)
        close(cmd.infile);
    if (cmd.outfile != STDOUT_FILENO)
        close(cmd.outfile);
    if (ret_value == 0 && shell.interrupt == FALSE)
        return wait_all_childs();
    shell.childs_pids.count = 0;
    if (shell.interrupt == TRUE)
    {
        shell.interrupt = FALSE;
        return 130;
    }
    return ret_value;
}