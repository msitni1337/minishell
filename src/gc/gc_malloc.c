/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 18:18:46 by nmellal           #+#    #+#             */
/*   Updated: 2024/05/11 20:42:58 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	add_end(t_mem **mem, t_mem *new)
{
	t_mem	*tmp;

	if (!*mem)
	{
		*mem = new;
		return ;
	}
	tmp = *mem;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
t_mem	*create_mem(void *ptr, t_bool is_perm)
{
	t_mem	*new;

	new = malloc(sizeof(t_mem));
	if (!new)
		return (NULL);
	new->ptr = ptr;
	new->is_freed = FALSE;
	new->next = NULL;
	new->is_perm = is_perm;
	return (new);
}

void	print_mem(t_mem *mem)
{
	t_mem	*tmp;

	tmp = mem;
	while (tmp)
	{
		printf("ptr: %p\n", tmp->ptr);
		tmp = tmp->next;
	}
}

t_mem	**get_mem()
{
	static t_mem *g_mem;

	return (&g_mem);
}

t_mem	*check_for_place(t_mem *mem, void *ptr)
{
	t_mem	*tmp;

	tmp = mem;
	while (tmp)
	{
		if (tmp->ptr == NULL || tmp->is_freed == TRUE)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	*gc_malloc(size_t size, t_bool is_perm)
{
	void	*ptr;
	t_mem	*new;
	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc failed and exiting");
		gc_exit(1);
	}
	ft_memset(ptr, 0, size);
	new = check_for_place(*get_mem(), ptr);
	if (new)
	{
		new->ptr = ptr;
		new->is_freed = FALSE;
		new->is_perm = is_perm;
		return (ptr);
	}
	else
		new = create_mem(ptr, is_perm);
	if (!new)
	{
		perror("malloc failed and exiting");
		free(ptr);
		gc_exit(1);
	}
	add_end(get_mem(), new);
	return (ptr);
}

int main(void)
{
	int	*p;
	int *p2;
	int *p3;

	p = gc_malloc(sizeof(int), FALSE);
	p2 = gc_malloc(sizeof(int), FALSE);
	p3 = gc_malloc(sizeof(int), FALSE);

	print_mem(*get_mem());
	gc_exit(0);
}