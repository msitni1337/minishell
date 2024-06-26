/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_or_replace_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:47:16 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 09:53:28 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

t_lstenv	*add_new_env(char *key, char *value)
{
	t_lstenv	*node;
	char		*key_cpy;
	char		*value_cpy;

	key_cpy = ft_strdup(key);
	if (key_cpy == NULL)
		return (NULL);
	if (value)
	{
		value_cpy = ft_strdup(value);
		if (value_cpy == NULL)
			return (free_p(key_cpy, NULL, NULL, NULL));
		node = add_env_end(&(g_shell.env_list), key_cpy, value_cpy);
	}
	else
	{
		node = add_env_end(&(g_shell.env_list), key_cpy, NULL);
		if (node)
			node->is_set = FALSE;
	}
	return (node);
}

t_lstenv	*replace_env(t_lstenv *node, char *value)
{
	char	*value_cpy;

	if (value)
	{
		value_cpy = ft_strdup(value);
		if (value_cpy == NULL)
			return (NULL);
		free_p(node->value, NULL, NULL, NULL);
		node->value = value_cpy;
		node->is_set = TRUE;
	}
	return (node);
}

t_lstenv	*add_or_replace_env(char *key, char *value)
{
	t_lstenv	*node;

	node = get_env_node(key);
	if (node)
		node = replace_env(node, value);
	else
		node = add_new_env(key, value);
	return (node);
}
