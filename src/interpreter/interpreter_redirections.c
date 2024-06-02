/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:52:11 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/02 18:52:12 by nmellal          ###   ########.fr       */
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

int	expand_filename_asterice(char *name, t_cmd *cmd, t_node *node)
{
	char	**expanded_name;
	size_t	count;
	int		ret_value;

	expanded_name = malloc(2 * sizeof(char *));
	if (expanded_name == NULL)
		malloc_error(name, NULL, NULL, NULL);
	count = 1;
	expanded_name[0] = ft_strdup(name);
	expanded_name[1] = NULL;
	if (expanded_name[0] == NULL)
		malloc_error(name, NULL, NULL, NULL);
	expanded_name = expand_asterices(expanded_name, &count);
	if (expanded_name == NULL)
		malloc_error(name, NULL, NULL, NULL);
	if (count > 1)
	{
		print_error(name, "ambiguous redirection");
		free_arr(expanded_name);
		return (1);
	}
	ret_value = open_file_as(*expanded_name, cmd, node->type);
	free_arr(expanded_name);
	return (ret_value);
}

int	open_file_from_node(t_node *node, t_cmd *cmd)
{
	char	*name;
	int		ret_value;

	if (node->type == NODE_HERE_DOC)
	{
		if (cmd->infile != STDIN_FILENO)
			close(cmd->infile);
		cmd->infile = node->children->here_doc_fd;
		return (0);
	}
	name = expand_string(node->children->token_str, TRUE);
	if (name == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	if (contains_chars(node->children->token_str, "*") == FALSE)
		ret_value = open_file_as(name, cmd, node->type);
	else
		ret_value = expand_filename_asterice(name, cmd, node);
	free(name);
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
