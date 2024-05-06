#include "interpreter.h"

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
        if (wait == FALSE)
            return 0;
        waitpid(pid, &ret_value, 0);
        WEXITSTATUS(ret_value);
        return ret_value;
    }
    else
    {
        // todo increment subshell level
        dup2(cmd.infile, STDIN_FILENO);
        dup2(cmd.outfile, STDOUT_FILENO);
        ret_value = interpret_root(cmd.subshell);
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
        ret_value = exec_builtin(cmd);
        exit(ret_value);
    }
    return 0;
}

int exec_bin(t_cmd cmd, int wait)
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
        if (wait == FALSE)
            return 0;
        waitpid(pid, &ret_value, 0);
        WEXITSTATUS(ret_value);
        return ret_value;
    }
    else
    {
        // todo increment subshell level
        dup2(cmd.infile, STDIN_FILENO);
        dup2(cmd.outfile, STDOUT_FILENO);
        // todo need to emplement exported envp to pass it to binary..

        execve(cmd.bin_path, cmd.argv, shell.exported_env);
        printf("##########%s##########\n", cmd.bin_path);
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