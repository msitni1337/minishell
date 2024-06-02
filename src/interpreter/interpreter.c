#include "interpreter.h"

int	parse_cmd(t_node *node, t_cmd *cmd)
{
	int	ret_value;

	if (cmd->outfile != STDOUT_FILENO)
	{
		close(cmd->outfile);
		cmd->outfile = STDOUT_FILENO;
	}
	ret_value = open_files(node, cmd);
	if (ret_value)
		return (ret_value);
	cmd->type = CMD_SUBSHELL;
	cmd->subshell = get_next_node_by_type(node->children, NODE_SUBSHELL);
	if (cmd->subshell == NULL)
	{
		cmd->type = CMD_BINARY;
		get_argv(node, cmd);
		if (cmd->argc == 0)
			return (0);
		if (is_builtin(cmd->argv[0]) == FALSE)
			return (get_cmd_path(cmd));
		cmd->type = CMD_BUILTIN;
	}
	return (0);
}

t_node	*advance_logical_operator(t_node *operator, int ret_value)
{
	t_node	*cmd_to_exec_next;

	cmd_to_exec_next = NULL;
	while (operator)
	{
		cmd_to_exec_next = operator->next;
		if (operator->type == NODE_AND && ret_value == 0)
			break ;
		if (operator->type == NODE_OR && ret_value)
			break ;
		operator = cmd_to_exec_next->next;
	}
	if (operator)
		return (cmd_to_exec_next);
	return (NULL);
}

int	interpret_logical_operator(t_node **curr_node, t_node *next_operator,
		t_cmd *cmd)
{
	int	ret_value;

	if (next_operator->type == NODE_PIPE)
	{
		ret_value = execute_piping(curr_node, cmd);
		*curr_node = advance_logical_operator(*curr_node, ret_value);
	}
	else
	{
		ret_value = parse_cmd(*curr_node, cmd);
		if (ret_value == 0)
			ret_value = execute_cmd(cmd, FALSE, TRUE);
		else
			free_cmd(cmd);
		*curr_node = advance_logical_operator(next_operator, ret_value);
	}
	return (ret_value);
}

t_cmd	init_cmd(void)
{
	t_cmd	cmd;

	cmd.read_pipe = -1;
	cmd.argv = NULL;
	cmd.bin_path = NULL;
	cmd.infile = STDIN_FILENO;
	cmd.outfile = STDOUT_FILENO;
	return (cmd);
}

int	interpret_root(t_node *root)
{
	t_node	*next_operator;
	t_cmd	cmd;
	int		ret_value;

	cmd = init_cmd();
	ret_value = 0;
	while (root && g_shell.interrupt == FALSE)
	{
		next_operator = root->next;
		if (next_operator)
			ret_value = interpret_logical_operator(&root, next_operator, &cmd);
		else
		{
			ret_value = parse_cmd(root, &cmd);
			if (ret_value == 0)
				ret_value = execute_cmd(&cmd, FALSE, TRUE);
			else
				free_cmd(&cmd);
			break ;
		}
	}
	close_here_docs();
	g_shell.interrupt = FALSE;
	return (ret_value);
}
