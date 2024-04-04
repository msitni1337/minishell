/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:44:18 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:44:20 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_write_out_nb(long nb, int fd)
{
	if (nb >= 0 && nb < 10)
	{
		ft_putchar_fd('0' + nb, fd);
		return ;
	}
	ft_write_out_nb(nb / 10, fd);
	ft_write_out_nb(nb % 10, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	nb;

	if (fd < 0)
		return ;
	nb = n;
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb *= -1;
	}
	ft_write_out_nb(nb, fd);
}

/*
int	main(int c, char**v)
{
	(void) c;
	ft_putnbr_fd(atoi(v[1]), 1);
}
*/
