/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:56:12 by nmellal           #+#    #+#             */
/*   Updated: 2024/05/18 01:14:53 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_lstenv *increment_shlvl()
{
	t_lstenv *node;
	char *value;
	int shlvl;

	node = get_env_node("SHLVL");
	if (node)
	{
		shlvl = ft_atoi(node->value);
		shlvl++;
		value = ft_itoa(shlvl);
		if (value == NULL)
			return NULL;
		node = add_or_replace_env("SHLVL", value);
		free(value);
	}
	else
		node = add_or_replace_env("SHLVL", "1");
	return node;
}

void take_env(const char **envp)
{
	char *key;
	char *value;
	t_lstenv *node;
	int i;

	g_shell.env_list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		if (key == NULL || value == NULL)
			malloc_error(key, value, NULL, NULL);
		node = add_env_end(&(g_shell.env_list), key, value);
		if (node == NULL)
			malloc_error(key, value, NULL, NULL);
		i++;
	}
	node = increment_shlvl();
	if (node == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
}
