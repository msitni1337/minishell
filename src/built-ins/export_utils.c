/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:44:23 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 09:44:24 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

void	ft_swap_env_lst(t_lstenv **p1, t_lstenv **p2)
{
	t_lstenv	*tmp;

	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

size_t	get_exported_env_count(void)
{
	t_lstenv	*env_lst;
	size_t		count;

	count = 0;
	env_lst = g_shell.env_list;
	while (env_lst)
	{
		count++;
		env_lst = env_lst->next;
	}
	return (count);
}

void	sort_env_list(t_lstenv **env_list)
{
	size_t	i;
	int		is_swapped;

	is_swapped = TRUE;
	while (is_swapped == TRUE)
	{
		is_swapped = FALSE;
		i = 1;
		while (env_list[i - 1] && env_list[i])
		{
			if (ft_strcmp(env_list[i - 1]->key, env_list[i]->key) > 0)
			{
				ft_swap_env_lst(&env_list[i - 1], &env_list[i]);
				is_swapped = TRUE;
			}
			i++;
		}
	}
}

t_lstenv	**get_sorted_env(void)
{
	t_lstenv	*env_lst;
	t_lstenv	**res;
	size_t		count;
	size_t		i;

	count = get_exported_env_count();
	env_lst = g_shell.env_list;
	res = malloc(sizeof(t_lstenv) * (count + 1));
	if (res)
	{
		i = 0;
		while (i < count)
		{
			res[i] = env_lst;
			env_lst = env_lst->next;
			i++;
		}
		res[i] = NULL;
		sort_env_list(res);
	}
	return (res);
}
