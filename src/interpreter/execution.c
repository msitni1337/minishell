#include "interpreter.h"

int exec_subshell(t_cmd cmd)
{
    int ret_value;
    int pid;

    pid = fork();
    if (pid == -1)
        exit_with_error();
    if (pid)
    {
        waitpid(pid, &ret_value, 0);
        WEXITSTATUS(ret_value);
        return ret_value;
    }
    else
    {
        // todo increment subshell level
        ret_value = interpret_root(cmd.subshell);
        gexit(ret_value);
    }
    return 0;
}

int exec_builtin(t_cmd cmd)
{
    int ret_value;

    if (!ft_strcmp(cmd.argv[0], "cd"))
    {
        ret_value = change_directory(cmd.argc, cmd.argv);
    }
    else if (!ft_strcmp(cmd.argv[0], "echo"))
    {
    }
    else if (!ft_strcmp(cmd.argv[0], "pwd"))
    {
    }
    else if (!ft_strcmp(cmd.argv[0], "export"))
    {
    }
    else if (!ft_strcmp(cmd.argv[0], "unset"))
    {
    }
    else if (!ft_strcmp(cmd.argv[0], "env"))
    {
    }
    else if (!ft_strcmp(cmd.argv[0], "exit"))
    {
    }
}

int execute_cmd(t_cmd cmd)
{
    if (cmd.is_subshell)
    {
        return exec_subshell(cmd);
    }
    else
    {
        if (cmd.is_builtin)
            return exec_builtin(cmd);
        else
            return exec_bin(cmd);
    }
}