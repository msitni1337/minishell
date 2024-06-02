/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:25:25 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:25:26 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"

void	*free_p(void *p1, void *p2, void *p3, char **p4)
{
	if (p1)
		free(p1);
	if (p2)
		free(p2);
	if (p3)
		free(p3);
	if (p4)
		free_arr(p4);
	return (NULL);
}

void	free_childs(t_node *parent)
{
	t_node	*tmp;
	t_node	*node;

	node = parent->children;
	while (node)
	{
		if (node->children)
			free_childs(node);
		tmp = node->next;
		free(node);
		node = tmp;
	}
}

void	free_tree(t_node **root)
{
	t_node	*tmp;
	t_node	*node;

	node = *root;
	while (node)
	{
		if (node->children)
			free_childs(node);
		tmp = node->next;
		free(node);
		node = tmp;
	}
	*root = NULL;
}

void	free_env_list(t_lstenv *head)
{
	t_lstenv	*node;
	t_lstenv	*tmp;

	node = head;
	while (node)
	{
		if (node->key)
			free(node->key);
		if (node->value)
			free(node->value);
		tmp = node->next;
		free(node);
		node = tmp;
	}
}

void	free_global_vars(void)
{
	free_p(g_shell.line, g_shell.childs_pids.data, NULL, NULL);
	g_shell.line = NULL;
	g_shell.childs_pids.data = NULL;
	g_shell.childs_pids.capacity = 0;
	free_env_list(g_shell.env_list);
	free_tree(&g_shell.tree_root);
	close_here_docs();
}
