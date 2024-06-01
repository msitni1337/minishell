/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:08:00 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/01 11:50:51 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

int	ft_unset(t_cmd cmd)
{
	int		ret_value;
	size_t	i;

	ret_value = 0;
	i = 1;
	while (cmd.argv[i])
	{
		if (ft_strlen(cmd.argv[i]))
		{
			if (check_key_is_valid(cmd.argv[i]))
			{
				key_not_valid("unset", cmd.argv[i]);
				ret_value = 1;
			}
			else
				remove_env(&g_shell.env_list, cmd.argv[i]);
		}
		i++;
	}
	return (ret_value);
}
