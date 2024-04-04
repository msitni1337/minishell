/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:40:25 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:40:31 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*ft_freelst(t_list *lst, void (*del)(void *))
{
	t_list	*tmp;

	tmp = lst;
	while (tmp)
	{
		if (del)
			del(tmp->content);
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*node;
	t_list	*head;

	head = 0;
	while (lst && f)
	{
		node = ft_lstnew(NULL);
		if (!node)
			return (ft_freelst(head, del));
		node->content = f(lst->content);
		ft_lstadd_back(&head, node);
		lst = lst->next;
	}
	return (head);
}
