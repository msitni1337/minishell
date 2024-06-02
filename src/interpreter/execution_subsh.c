/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_subsh.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:51:53 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/02 18:51:54 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

void	exec_subshell_child(t_cmd *cmd)
{
	int	ret_value;

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

int	exec_subshell(t_cmd *cmd, int wait_child)
{
	int	pid;

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
		return (0);
	return (wait_all_childs());
}
