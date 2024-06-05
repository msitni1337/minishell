/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_argv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:52:02 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/05 21:23:06 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

size_t	get_argc(t_node *cmd_node)
{
	size_t	argc;

	argc = 0;
	cmd_node = get_next_node_by_type(cmd_node->children, NODE_STR);
	while (cmd_node)
	{
		argc++;
		cmd_node = get_next_node_by_type(cmd_node->next, NODE_STR);
	}
	return (argc);
}

char	*expand_argv(t_string str, int *has_asterix)
{
	char	*res;

	res = expand_string(str, EXPAND_VARS);
	if (ft_strchr(res, '*'))
		*has_asterix = TRUE;
	return (res);
}

void	get_argv(t_node *cmd_node, t_cmd *cmd)
{
	t_node	*tmp;
	int		has_asterix;
	size_t	i;

	cmd->bin_path = NULL;
	cmd->argc = get_argc(cmd_node);
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	cmd->argv[cmd->argc] = NULL;
	i = 0;
	has_asterix = FALSE;
	tmp = get_next_node_by_type(cmd_node->children, NODE_STR);
	while (tmp)
	{
		cmd->argv[i] = expand_argv(tmp->token_str, &has_asterix);
		i++;
		tmp = get_next_node_by_type(tmp->next, NODE_STR);
	}
	cmd->argv = expand_args(cmd->argv, &cmd->argc);
	if (cmd->argv == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	if (has_asterix == FALSE)
		return ;
	cmd->argv = expand_asterices(cmd->argv, &cmd->argc);
	if (cmd->argv == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
}
