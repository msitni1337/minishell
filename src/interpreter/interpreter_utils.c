#include "interpreter.h"

t_node	*get_next_node_by_type(t_node *root, t_node_type type)
{
	t_node	*tmp;

	tmp = root;
	while (tmp && !(tmp->type & type))
		tmp = tmp->next;
	return (tmp);
}

void	get_perm_flags(int *p_flags, int *m_flags, t_node_type type)
{
	if (type == NODE_REDIRECT_IN)
	{
		*p_flags = O_RDONLY;
		*m_flags = 0;
	}
	else if (type == NODE_REDIRECT_OUT)
	{
		*p_flags = O_WRONLY | O_CREAT | O_TRUNC;
		*m_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	}
	else if (type == NODE_APPEND)
	{
		*p_flags = O_WRONLY | O_APPEND | O_CREAT;
		*m_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	}
}

int	is_builtin(const char *s)
{
	return (!ft_strcmp(s, "cd") || !ft_strcmp(s, "echo") || !ft_strcmp(s, "pwd")
		|| !ft_strcmp(s, "export") || !ft_strcmp(s, "unset") || !ft_strcmp(s,
			"env") || !ft_strcmp(s, "exit"));
}
