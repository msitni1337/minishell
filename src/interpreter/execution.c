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
        WEXITSTATUS(ret_value);
        i++;
    }
    return ret_value;
}

int exec_subshell(t_cmd cmd, int wait)
{
    int ret_value;
    int pid;
    
    pid = fork();
    if (pid == -1)
    {
        perror(cmd.argv[0]);
        exit(errno);
    }
    if (pid)
    {
        add_to_arr(&(shell.childs_pids), &pid);
        if (wait == FALSE)
            return 0;
        if (cmd.infile != STDIN_FILENO)
            close(cmd.infile);
        if (cmd.outfile != STDOUT_FILENO)
            close(cmd.outfile);
        return wait_all_childs();
    }
    else
    {
        // todo increment subshell level
        if (cmd.infile != STDIN_FILENO)
            dup2(cmd.infile, STDIN_FILENO);
        if (cmd.outfile != STDOUT_FILENO)
            dup2(cmd.outfile, STDOUT_FILENO);
        if (cmd.read_pipe != -1)
            close(cmd.read_pipe);
        ret_value = interpret_root(cmd.subshell->children);
        exit(ret_value);
    }
    return 0;
}

int exec_builtin(t_cmd cmd)
{
    int ret_value;

    if (!ft_strcmp(cmd.argv[0], "cd"))
        ret_value = change_directory(cmd);
    else if (!ft_strcmp(cmd.argv[0], "echo"))
        ret_value = ft_echo(cmd.argc, cmd.argv);
    else if (!ft_strcmp(cmd.argv[0], "pwd"))
        ret_value = ft_pwd(cmd.argc, cmd.argv);
    else if (!ft_strcmp(cmd.argv[0], "export"))
        assert(!"NOT IMPLEMENTED");
    else if (!ft_strcmp(cmd.argv[0], "unset"))
        assert(!"NOT IMPLEMENTED");
    else if (!ft_strcmp(cmd.argv[0], "env"))
        assert(!"NOT IMPLEMENTED");
    else if (!ft_strcmp(cmd.argv[0], "exit"))
        assert(!"NOT IMPLEMENTED");
    return ret_value;
}

int exec_builtin_no_wait(t_cmd cmd)
{
    int ret_value;
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror(cmd.argv[0]);
        exit(errno);
    }
    if (pid == 0)
    {
        if (cmd.read_pipe != -1)
            close(cmd.read_pipe);
        ret_value = exec_builtin(cmd);
        exit(ret_value);
    }
    return 0;
}

int exec_bin(t_cmd cmd, int wait)
{
    int pid;

    pid = fork();
    if (pid == -1)
    {
        perror(cmd.argv[0]);
        exit(errno);
    }
    if (pid)
    {
        add_to_arr(&(shell.childs_pids), &pid);
        if (wait == FALSE)
            return 0;
        if (cmd.infile != STDIN_FILENO)
            close(cmd.infile);
        if (cmd.outfile != STDOUT_FILENO)
            close(cmd.outfile);
        return wait_all_childs();
    }
    else
    {
        // todo increment subshell level
        if (cmd.infile != STDIN_FILENO)
            dup2(cmd.infile, STDIN_FILENO);
        if (cmd.outfile != STDOUT_FILENO)
            dup2(cmd.outfile, STDOUT_FILENO);

        if (cmd.read_pipe != -1)
            close(cmd.read_pipe);

        // todo need to emplement exported envp to pass it to binary..

        execve(cmd.bin_path, cmd.argv, shell.exported_env);
        printf("%s : execve failed\n", cmd.bin_path);
        perror(cmd.argv[0]);
        exit(errno);
    }
    return 0;
}

int execute_cmd(t_cmd cmd, int wait)
{
    if (cmd.type == CMD_SUBSHELL)
        return exec_subshell(cmd, wait);
    else if (cmd.type == CMD_BINARY)
        return exec_bin(cmd, wait);
    else if (cmd.type == CMD_BUILTIN)
    {
        if (wait == TRUE)
            return exec_builtin(cmd);
        else
            return exec_builtin_no_wait(cmd);
    }
    assert(!"IMPOSSIBLE TO REACH");
    return 0;
}