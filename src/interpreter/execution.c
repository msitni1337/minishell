#include "interpreter.h"

int wait_all_childs()
{
    int ret_value;
    int *pids;
    size_t i;

    pids = shell.childs_pids.data;
    ret_value = 0;
    i = 0;
    while (i < shell.childs_pids.count)
    {
        waitpid(pids[i], &ret_value, 0);
        ret_value = WEXITSTATUS(ret_value);
        i++;
    }
    shell.childs_pids.count = 0;
    if (shell.interrupt == TRUE)
    {
        // shell.interrupt = FALSE;
        return 130;
    }
    return ret_value;
}

int exec_subshell(t_cmd *cmd, bool wait_child)
{
    int ret_value;
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror(cmd->argv[0]);
        exit_with_code(cmd, errno);
    }
    if (pid)
    {
        if (add_to_arr(&(shell.childs_pids), &pid) == NULL)
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
    else
    {
        if (cmd->infile != STDIN_FILENO)
            dup2(cmd->infile, STDIN_FILENO);
        if (cmd->outfile != STDOUT_FILENO)
            dup2(cmd->outfile, STDOUT_FILENO);
        if (cmd->read_pipe != -1)
            close(cmd->read_pipe);
        ret_value = interpret_root(cmd->subshell->children);
        exit_with_code(cmd, ret_value);
    }
    return 0;
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

int exec_builtin_fork(t_cmd *cmd)
{
    int ret_value;
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror(cmd->argv[0]);
        exit_with_code(cmd, errno);
    }
    if (pid)
    {
        if (add_to_arr(&(shell.childs_pids), &pid) == NULL)
            malloc_error(NULL, NULL, NULL, cmd);
        if (add_or_replace_env("_", cmd->argv[0]) == NULL)
            malloc_error(NULL, NULL, NULL, cmd);
        if (cmd->infile != STDIN_FILENO)
            close(cmd->infile);
        if (cmd->outfile != STDOUT_FILENO)
            close(cmd->outfile);
        free_cmd(cmd);
    }
    else
    {
        if (cmd->read_pipe != -1)
            close(cmd->read_pipe);
        ret_value = exec_builtin(cmd);
        exit_with_code(NULL, ret_value);
    }
    return 0;
}

int exec_bin(t_cmd *cmd, bool wait_child)
{
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror(cmd->argv[0]);
        exit_with_code(cmd, errno);
    }
    if (pid)
    {
        if (add_to_arr(&(shell.childs_pids), &pid) == NULL)
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
    else
    {
        // todo increment subshell level
        if (cmd->infile != STDIN_FILENO)
            dup2(cmd->infile, STDIN_FILENO);
        if (cmd->outfile != STDOUT_FILENO)
            dup2(cmd->outfile, STDOUT_FILENO);

        if (cmd->read_pipe != -1)
            close(cmd->read_pipe);

        // todo need to emplement exported envp to pass it to binary..
        char **envp = get_exported_env_arr();
        if (envp == NULL)
            malloc_error(NULL, NULL, NULL, cmd);
        // printf("cmd is = %s\n", cmd->bin_path);
        if (cmd->argc > 0)
        {
            execve(cmd->bin_path, cmd->argv, envp);
            perror("Execve failed.");
        }
        free_arr(envp);
        exit_with_code(cmd, EXIT_SUCCESS);
        // printf("%s : execve failed\n", cmd->bin_path);
        // perror(cmd->argv[0]);
    }
    return 0;
}

int execute_cmd(t_cmd *cmd, bool is_pipe, bool wait_child)
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