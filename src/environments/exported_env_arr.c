/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exported_env_arr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:42 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 09:52:14 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

char	*construct_env(char *key, char *value)
{
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (tmp == NULL)
		return (NULL);
	while (key && *key)
	{
		tmp[i] = *key;
		key++;
		i++;
	}
	tmp[i] = '=';
	i++;
	while (value && *value)
	{
		tmp[i] = *value;
		value++;
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}

size_t	exp_env_size(void)
{
	t_lstenv	*lst;
	size_t		size;

	size = 0;
	lst = g_shell.env_list;
	while (lst)
	{
		if (lst->is_set == TRUE)
			size++;
		lst = lst->next;
	}
	return (size);
}

char	**get_exported_env_arr(void)
{
	t_lstenv	*current;
	char		**res;
	size_t		i;

	res = malloc(sizeof(char *) * (exp_env_size() + 1));
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, sizeof(char *) * (exp_env_size() + 1));
	current = g_shell.env_list;
	i = 0;
	while (current)
	{
		if (current->is_set == TRUE)
		{
			res[i] = construct_env(current->key, current->value);
			if (res[i] == NULL)
				return (free_p(NULL, NULL, NULL, res));
			i++;
		}
		current = current->next;
	}
	res[i] = NULL;
	return (res);
}
