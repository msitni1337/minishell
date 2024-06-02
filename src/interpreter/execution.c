#include "interpreter.h"

int wait_all_childs(void)
{
	int ret_value;
	int *pids;
	size_t i;

	pids = g_shell.childs_pids.data;
	ret_value = 0;
	i = 0;
	while (i < g_shell.childs_pids.count)
	{
		waitpid(pids[i], &ret_value, 0);
		ret_value = WEXITSTATUS(ret_value);
		i++;
	}
	g_shell.childs_pids.count = 0;
	if (g_shell.interrupt == TRUE)
		return (130);
	return (ret_value);
}

int execute_cmd(t_cmd *cmd, int is_pipe, int wait_child)
{
	if (cmd->type == CMD_SUBSHELL)
		return (exec_subshell(cmd, wait_child));
	else if (cmd->type == CMD_BINARY)
		return (exec_bin(cmd, wait_child));
	else if (cmd->type == CMD_BUILTIN)
	{
		if (is_pipe == TRUE)
			return (exec_builtin_fork(cmd));
		else
			return (exec_builtin(cmd));
	}
	return (0);
}
