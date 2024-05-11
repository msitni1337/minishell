/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 20:25:39 by nmellal           #+#    #+#             */
/*   Updated: 2024/05/11 20:53:22 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"


void	free_mem(t_mem *mem)
{
	t_mem	*tmp;
	t_mem	*next;

	tmp = mem;
	while (tmp)
	{
		if (tmp->is_freed == FALSE)
			free(tmp->ptr);
		tmp = tmp->next;
	}
	tmp = mem;
	while (tmp)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}
void	gc_exit(int exit_status)
{
	t_mem * mem;

	mem = *get_mem();
	free_mem(mem);
	exit(exit_status);
}

void	free_not_perm(t_mem *mem)
{
	t_mem *tmp;

	tmp = mem;
	while (tmp)
	{
		if (tmp->is_freed == FALSE && tmp->is_perm == FALSE)
		{
			free(tmp->ptr);
			tmp->is_freed = TRUE;
		}
		tmp = tmp->next;
	}
}

void	gc_free_all(void)
{
	t_mem	**mem;

	mem = get_mem();
	free_not_perm(*mem);
	*mem = NULL;
}

void	gc_free(void *ptr)
{
	t_mem	**mem;
	t_mem	*tmp;

	mem = get_mem();
	tmp = *mem;
	while (tmp)
	{
		if (tmp->ptr == ptr)
		{
			free(tmp->ptr);
			tmp->is_freed = TRUE;
			return ;
		}
		tmp = tmp->next;
	}
}