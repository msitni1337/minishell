#include "interpreter.h"

void	run_bin(t_cmd *cmd)
{
	char	**envp;

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

void	exec_bin_child(t_cmd *cmd)
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

int	exec_bin(t_cmd *cmd, int wait_child)
{
	int	pid;

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
		return (0);
	return (wait_all_childs());
}
