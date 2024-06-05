/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:52:11 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/05 21:29:41 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

void	close_previous_files(t_cmd *cmd, t_node_type type)
{
	if (cmd->infile != STDIN_FILENO && type == NODE_REDIRECT_IN)
	{
		close(cmd->infile);
		cmd->infile = STDIN_FILENO;
	}
	else if (cmd->outfile != STDOUT_FILENO && (type == NODE_REDIRECT_OUT
			|| type == NODE_APPEND))
	{
		close(cmd->outfile);
		cmd->outfile = STDOUT_FILENO;
	}
}

int	open_file_as(char *fname, t_cmd *cmd, t_node_type type)
{
	int	fd;
	int	p_flags;
	int	m_flags;

	close_previous_files(cmd, type);
	get_perm_flags(&p_flags, &m_flags, type);
	fd = open(fname, p_flags, m_flags);
	if (fd < 0)
	{
		perror(fname);
		return (1);
	}
	if (type == NODE_REDIRECT_IN)
		cmd->infile = fd;
	if (type == NODE_REDIRECT_OUT || type == NODE_APPEND)
		cmd->outfile = fd;
	return (0);
}

char	**get_filename_from_node(t_node *node, size_t *count)
{
	char	**expanded_name;
	char	*tmp;

	expanded_name = malloc(sizeof(char *) * 2);
	if (expanded_name == NULL)
		return (NULL);
	expanded_name[1] = NULL;
	tmp = expand_string(node->children->token_str, EXPAND_VARS);
	if (tmp == NULL)
		return (free_p(expanded_name, NULL, NULL, NULL));
	expanded_name[0] = tmp;
	*count = 1;
	expanded_name = expand_args(expanded_name, count);
	if (expanded_name == NULL)
		return (NULL);
	if (*count > 1)
		return (expanded_name);
	if (ft_strchr(expanded_name[0], '*'))
		return (expand_asterices(expanded_name, count));
	return (expanded_name);
}

int	open_file_from_node(t_node *node, t_cmd *cmd)
{
	char	**expanded_name;
	size_t	count;
	int		ret_value;

	if (node->type == NODE_HERE_DOC)
	{
		if (cmd->infile != STDIN_FILENO)
			close(cmd->infile);
		cmd->infile = node->children->here_doc_fd;
		return (0);
	}
	expanded_name = get_filename_from_node(node, &count);
	if (expanded_name == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	if (count > 1)
	{
		print_error_str(node->children->token_str, "ambiguous redirect");
		free_arr(expanded_name);
		return (1);
	}
	ret_value = open_file_as(*expanded_name, cmd, node->type);
	free_arr(expanded_name);
	return (ret_value);
}

int	open_files(t_node *cmd_node, t_cmd *cmd)
{
	int		ret_value;
	t_node	*node;

	node = get_next_node_by_type(cmd_node->children,
			NODE_REDIRECT_IN | NODE_REDIRECT_OUT | NODE_APPEND | NODE_HERE_DOC);
	while (node)
	{
		ret_value = open_file_from_node(node, cmd);
		if (ret_value)
			return (ret_value);
		node = get_next_node_by_type(node->next,
				NODE_REDIRECT_IN | NODE_REDIRECT_OUT
				| NODE_APPEND | NODE_HERE_DOC);
	}
	return (0);
}
