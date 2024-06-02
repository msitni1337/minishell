/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_manipulations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:49:51 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 09:52:42 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

t_lstenv	*create_nodes(char *key, char *value)
{
	t_lstenv	*new_node;

	new_node = malloc(sizeof(t_lstenv));
	if (new_node == NULL)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->is_set = TRUE;
	new_node->next = NULL;
	return (new_node);
}

t_lstenv	*add_env_end(t_lstenv **head, char *key, char *value)
{
	t_lstenv	*current;

	if (!*head)
	{
		*head = create_nodes(key, value);
		return (*head);
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = create_nodes(key, value);
	return (current->next);
}

void	remove_env(t_lstenv **lstenv, char *key)
{
	t_lstenv	*current;
	t_lstenv	*prev;

	current = *lstenv;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*lstenv = current->next;
			free_p(current->key, current->value, current, NULL);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

t_lstenv	*get_env_node(char *key)
{
	t_lstenv	*current;

	current = g_shell.env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*get_env_value(char *key)
{
	t_lstenv	*current;

	current = g_shell.env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
