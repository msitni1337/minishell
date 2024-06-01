/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:48:42 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/01 11:50:51 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

int	ft_env(t_cmd cmd)
{
	t_lstenv*head;

	head = shell.env_list;
	while (head)
	{
		if (head->is_set)
		{
			ft_putstr_fd(head->key, cmd.outfile);
			write(cmd.outfile, "=", 1);
			ft_putendl_fd(head->value, cmd.outfile);
		}
		head = head->next;
	}
	return 0;
}