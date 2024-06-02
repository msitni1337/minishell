/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:51:49 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/02 18:51:50 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

int	run_builtin(t_cmd *cmd)
{
	int	ret_value;

	ret_value = 0;
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
	return (ret_value);
}

int	exec_builtin(t_cmd *cmd)
{
	int	ret_value;

	ret_value = 0;
	if (cmd->argc > 0)
		if (add_or_replace_env("_", cmd->argv[cmd->argc - 1]) == NULL)
			malloc_error(NULL, NULL, NULL, cmd);
	ret_value = run_builtin(cmd);
	if (cmd->infile != STDIN_FILENO)
		close(cmd->infile);
	if (cmd->outfile != STDOUT_FILENO)
		close(cmd->outfile);
	free_cmd(cmd);
	return (ret_value);
}

void	exec_builtin_fork_child(t_cmd *cmd)
{
	int	ret_value;

	if (cmd->read_pipe != -1)
		close(cmd->read_pipe);
	ret_value = exec_builtin(cmd);
	exit_with_code(NULL, ret_value);
}

int	exec_builtin_fork(t_cmd *cmd)
{
	int	pid;

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
	if (cmd->infile != STDIN_FILENO)
		close(cmd->infile);
	if (cmd->outfile != STDOUT_FILENO)
		close(cmd->outfile);
	free_cmd(cmd);
	return (0);
}
