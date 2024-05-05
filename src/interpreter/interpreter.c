#include "interpreter.h"

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

    assert(type != NODE_HERE_DOC);
    assert(type != NODE_APPEND);
    if (cmd->infile != 0)
        close(cmd->infile);
    if (cmd->outfile != 1)
        close(cmd->outfile);
    fd = open(fname, O_RDONLY);
    if (fd < 0)
    {
        perror(fname);
        return fd;
    }
    if (type == NODE_REDIRECT_IN)
        cmd->infile = fd;
    if (type == NODE_REDIRECT_OUT)
        cmd->outfile = fd;
    return 0;
}

// todo return all fds.
int open_files(t_node *cmd_node, t_cmd *cmd)
{
    t_node *node;
    int fd;

    fd = 0;
    // todo Protect cmd->children
    node = get_next_node_by_type(cmd_node->children, NODE_REDIRECT_IN | NODE_REDIRECT_OUT | NODE_APPEND | NODE_HERE_DOC);
    while (node)
    {
        char *name = ft_substr(node->children->token_str.s, 0, node->children->token_str.count);
        assert(node->type != NODE_HERE_DOC);
        assert(node->type != NODE_APPEND);
        if (open_file_as(name, cmd, node->type))
            return -1;
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
        cmd_node = get_next_node_by_type(cmd_node, NODE_STRING);
    }
    return argc;
}

void get_cmd(t_node *cmd_node, t_cmd *cmd)
{
    t_node *tmp;
    size_t i;
    cmd->argc = get_argc(cmd_node);
    cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));

    i = 0;
    tmp = get_next_node_by_type(cmd_node->children, NODE_STRING);
    while (tmp)
    {
        cmd->argv[i] = ft_substr(tmp->token_str.s, 0, tmp->token_str.count);
        i++;
        tmp = get_next_node_by_type(tmp, NODE_STRING);
    }
}

int is_builtin(const char *s)
{
    return !ft_strcmp(s, "cd") || !ft_strcmp(s, "echo") || !ft_strcmp(s, "pwd") || !ft_strcmp(s, "export") || !ft_strcmp(s, "unset") || !ft_strcmp(s, "env") || !ft_strcmp(s, "exit");
}

int check_bin_path(char **argv)
{
    char *argv0;

    argv0 = *argv;
    if (ft_strchr(argv0, '/') && access(argv0, X_OK))
    {
        perror("bash");
        return errno;
    }
    /*
        todo : join with path
    */
    return 0;
}

int parse_cmd(t_node *node, t_cmd *cmd)
{
    t_node *subshell;
    
    open_files(node, cmd);
    subshell = get_next_node_by_type(node->children, NODE_SUBSHELL); 
    if (subshell)
    {
        cmd->subshell = subshell;
        cmd->is_subshell = TRUE;
    }
    else
    {
        cmd->is_subshell = FALSE;
        get_cmd(node, cmd);
        if (cmd->argc > 0)
        {
            if (is_builtin(cmd->argv[0]))
                return 0;
            return check_bin_path(cmd->argv);
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
    size_t i;

    node = root;
    while (node)
    {
        ret_value = parse_cmd(node, &cmd);
        node = node->next;
        if (node)
        {
            if (node->type == NODE_PIPE)
            {
                if (pipe(pip) == -1)
                    exitwitherror();
                if (!ret_value)
                {
                    if (cmd.outfile != 1)
                        close(cmd.outfile);
                    cmd.outfile = pip[1];
                    execute_cmd_async(cmd);
                    cmd.infile = pip[0]; // this gonna serve for the next cmd
                }
                else
                {
                    close(pip[1]);
                    cmd.infile = pip[0]; // this gonna serve for the next cmd
                }
            }
            else if (node->type == NODE_AND)
            {
                if (ret_value)
                    break;
                else
                {
                    ret_value = execute_cmd(cmd);
                    if (ret_value)
                        break;
                }
            }
            else if (node->type == NODE_OR && !ret_value)
            {
                ret_value = execute_cmd(cmd);
                if (!ret_value)
                    break;
            }
            node = node->next;
        }
        else if (!ret_value)
        {
            ret_value = execute_cmd(cmd);
        }
    }

    return ret_value;
}