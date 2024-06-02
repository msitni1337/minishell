/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:07:29 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:07:34 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_node	*create_node(int type)
{
	t_node	*res;

	res = malloc(sizeof(t_node));
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, sizeof(t_node));
	res->type = type;
	return (res);
}

void	append_node(t_node **parent, t_node *child)
{
	t_node	*node;

	if (*parent == NULL)
	{
		*parent = child;
		child->list_count++;
		return ;
	}
	(*parent)->list_count++;
	node = *parent;
	while (node->next)
		node = node->next;
	node->next = child;
}

void	append_child(t_node *parent, t_node *child)
{
	t_node	**tmp;
	t_node	*node;

	if (parent == NULL)
		return ;
	parent->childs_count++;
	tmp = &(parent->children);
	node = parent->children;
	while (node)
	{
		tmp = &(node->next);
		node = node->next;
	}
	*tmp = child;
}
