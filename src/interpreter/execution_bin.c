/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_bin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:51:44 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/02 18:51:45 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

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

void handle_sig_quit(int sig)
{
	(void)sig;
	int *pids;
	size_t i;

	pids = g_shell.childs_pids.data;
	i = 0;
	while (i < g_shell.childs_pids.count)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	ft_putendl_fd("Quit (core dumb)", STDERR_FILENO);
}

void exec_bin_main(t_cmd *cmd)
{
	if (cmd->infile != STDIN_FILENO)
		close(cmd->infile);
	if (cmd->outfile != STDOUT_FILENO)
		close(cmd->outfile);
	free_cmd(cmd);
	if (signal(SIGQUIT, handle_sig_quit) == SIG_ERR)
	{
		print_error(NULL, "Can't set signal.");
		exit_with_code(NULL, EXIT_FAILURE);
	}
}

int exec_bin(t_cmd *cmd, int wait_child)
{
	int pid;

	if (cmd->argc > 0)
		if (add_or_replace_env("_", cmd->argv[cmd->argc - 1]) == NULL)
			malloc_error(NULL, NULL, NULL, cmd);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit_with_code(cmd, EXIT_FAILURE);
	}
	if (pid == 0)
		exec_bin_child(cmd);
	if (add_to_arr(&(g_shell.childs_pids), &pid) == NULL)
		malloc_error(NULL, NULL, NULL, cmd);
	exec_bin_main(cmd);
	if (wait_child == FALSE)
		return (0);
	return (wait_all_childs());
}
