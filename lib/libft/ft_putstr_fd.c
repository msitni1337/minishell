/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:44:39 by msitni            #+#    #+#             */
/*   Updated: 2024/05/18 19:37:38 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	unsigned int	count;

	count = 0;
	while (fd >= 0 && s && s[count])
		count++;
	if (fd >= 0 && s)
		write(fd, s, count);
}
