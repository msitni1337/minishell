#include "interpreter.h"

int wait_all_childs()
{
    int ret_value;
    int *pids;
    size_t i;

    pids = g_shell.childs_pids.data;
    ret_value = 0;
    i = 0;
    while (i < g_shell.childs_pids.count)
    {
        waitpid(pids[i], &ret_value, 0);
        ret_value = WEXITSTATUS(ret_value);
        i++;
    }
    g_shell.childs_pids.count = 0;
    if (g_shell.interrupt == TRUE)
        return 130;
    return ret_value;
}

void exec_subshell_child(t_cmd *cmd)
{
    int ret_value;

    if (cmd->infile != STDIN_FILENO)
    {
        if (dup2(cmd->infile, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit_with_code(cmd, EXIT_FAILURE);
        }
        close(cmd->infile);
    }
    if (cmd->outfile != STDOUT_FILENO)
    {
        if (dup2(cmd->outfile, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit_with_code(cmd, EXIT_FAILURE);
        }
        close(cmd->outfile);
    }
    if (cmd->read_pipe != -1)
        close(cmd->read_pipe);
    ret_value = interpret_root(cmd->subshell->children);
    exit_with_code(cmd, ret_value);
}

int exec_subshell(t_cmd *cmd, int wait_child)
{
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit_with_code(cmd, EXIT_FAILURE);
    }
    if (pid == 0)
        exec_subshell_child(cmd);
    if (add_to_arr(&(g_shell.childs_pids), &pid) == NULL)
        malloc_error(NULL, NULL, NULL, cmd);
    if (cmd->infile != STDIN_FILENO)
        close(cmd->infile);
    if (cmd->outfile != STDOUT_FILENO)
        close(cmd->outfile);
    free_cmd(cmd);
    if (wait_child == FALSE)
        return 0;
    return wait_all_childs();
}

int exec_builtin(t_cmd *cmd)
{
    int ret_value;

    ret_value = 0;
    if (add_or_replace_env("_", cmd->argv[0]) == NULL)
        malloc_error(NULL, NULL, NULL, cmd);
    if (!ft_strcmp(cmd->argv[0], "cd"))
        ret_value = change_directory(*cmd);
    else if (!ft_strcmp(cmd->argv[0], "echo"))
        ret_value = ft_echo(*cmd);
    else if (!ft_strcmp(cmd->argv[0], "pwd"))
        ret_value = ft_pwd(*cmd);
    else if (!ft_strcmp(cmd->argv[0], "export"))
        ret_value = ft_export(*cmd);
    else if (!ft_strcmp(cmd->argv[0], "unset"))
        ret_value = ft_unset(*cmd);
    else if (!ft_strcmp(cmd->argv[0], "env"))
        ret_value = ft_env(*cmd);
    else if (!ft_strcmp(cmd->argv[0], "exit"))
        ret_value = ft_exit(*cmd);
    if (cmd->infile != STDIN_FILENO)
        close(cmd->infile);
    if (cmd->outfile != STDOUT_FILENO)
        close(cmd->outfile);
    free_cmd(cmd);
    return ret_value;
}

void exec_builtin_fork_child(t_cmd *cmd)
{
    int ret_value;

    if (cmd->read_pipe != -1)
        close(cmd->read_pipe);
    ret_value = exec_builtin(cmd);
    exit_with_code(NULL, ret_value);
}

int exec_builtin_fork(t_cmd *cmd)
{
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit_with_code(cmd, errno);
    }
    if (pid == 0)
        exec_builtin_fork_child(cmd);
    if (add_to_arr(&(g_shell.childs_pids), &pid) == NULL)
        malloc_error(NULL, NULL, NULL, cmd);
    if (add_or_replace_env("_", cmd->argv[0]) == NULL)
        malloc_error(NULL, NULL, NULL, cmd);
    if (cmd->infile != STDIN_FILENO)
        close(cmd->infile);
    if (cmd->outfile != STDOUT_FILENO)
        close(cmd->outfile);
    free_cmd(cmd);
    return 0;
}

void run_bin(t_cmd *cmd)
{
    char **envp;
    
    envp = get_exported_env_arr();
    if (envp == NULL)
        malloc_error(NULL, NULL, NULL, cmd);
    if (cmd->argc > 0)
    {
        execve(cmd->bin_path, cmd->argv, envp);
        perror("execve");
    }
    free_arr(envp);
    exit_with_code(cmd, EXIT_SUCCESS);
}

void exec_bin_child(t_cmd *cmd)
{
    if (cmd->infile != STDIN_FILENO)
    {
        if (dup2(cmd->infile, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit_with_code(cmd, EXIT_FAILURE);
        }
        close(cmd->infile);
    }
    if (cmd->outfile != STDOUT_FILENO)
    {
        if (dup2(cmd->outfile, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit_with_code(cmd, EXIT_FAILURE);
        }
        close(cmd->outfile);
    }
    if (cmd->read_pipe != -1)
        close(cmd->read_pipe);
    run_bin(cmd);
}

int exec_bin(t_cmd *cmd, int wait_child)
{
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit_with_code(cmd, errno);
    }
    if (pid == 0)
        exec_bin_child(cmd);
    if (add_to_arr(&(g_shell.childs_pids), &pid) == NULL)
        malloc_error(NULL, NULL, NULL, cmd);
    if (add_or_replace_env("_", cmd->bin_path) == NULL)
        malloc_error(NULL, NULL, NULL, cmd);
    if (cmd->infile != STDIN_FILENO)
        close(cmd->infile);
    if (cmd->outfile != STDOUT_FILENO)
        close(cmd->outfile);
    free_cmd(cmd);
    if (wait_child == FALSE)
        return 0;
    return wait_all_childs();
}

int execute_cmd(t_cmd *cmd, int is_pipe, int wait_child)
{
    if (cmd->type == CMD_SUBSHELL)
        return exec_subshell(cmd, wait_child);
    else if (cmd->type == CMD_BINARY)
        return exec_bin(cmd, wait_child);
    else if (cmd->type == CMD_BUILTIN)
    {
        if (is_pipe == TRUE)
            return exec_builtin_fork(cmd);
        else
            return exec_builtin(cmd);
    }
    return 0;
}