#include "interpreter.h"

int	wait_on_last_piped_cmd(t_cmd *cmd, int ret_value)
{
	if (ret_value == 0)
	{
		execute_cmd(cmd, TRUE, FALSE);
		ret_value = wait_all_childs();
	}
	else
	{
		if (cmd->infile != STDIN_FILENO)
			close(cmd->infile);
		if (cmd->outfile != STDOUT_FILENO)
			close(cmd->outfile);
		free_cmd(cmd);
		wait_all_childs();
	}
	return (ret_value);
}

void	queue_cmd(t_cmd *cmd, int ret_value)
{
	int	pip[2];

	if (pipe(pip) == -1)
	{
		perror(cmd->argv[0]);
		exit_with_code(cmd, EXIT_FAILURE);
	}
	if (ret_value == 0)
	{
		if (cmd->outfile == STDOUT_FILENO)
			cmd->outfile = pip[1];
		else
			close(pip[1]);
		cmd->read_pipe = pip[0];
		execute_cmd(cmd, TRUE, FALSE);
		cmd->infile = pip[0];
	}
	else
	{
		close(pip[1]);
		if (cmd->infile != STDIN_FILENO)
			close(cmd->infile);
		cmd->infile = pip[0];
		free_cmd(cmd);
	}
}

int	execute_piping(t_node **node, t_cmd *cmd)
{
	int	ret_value;

	cmd->infile = STDIN_FILENO;
	cmd->outfile = STDOUT_FILENO;
	while (TRUE)
	{
		ret_value = parse_cmd(*node, cmd);
		(*node) = (*node)->next;
		if (*node == NULL || (*node)->type != NODE_PIPE)
			break ;
		queue_cmd(cmd, ret_value);
		*node = (*node)->next;
	}
	return (wait_on_last_piped_cmd(cmd, ret_value));
}
