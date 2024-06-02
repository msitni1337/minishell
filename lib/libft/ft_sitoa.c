/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:12:56 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 10:14:57 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	copy_num_to_str(char *str, size_t *i, long nbr)
{
	long	tmp;

	if (nbr < 0)
	{
		nbr = -nbr;
		str[*i] = '-';
		(*i)++;
	}
	if (nbr >= 0 && nbr <= 9)
	{
		tmp = nbr;
		while (tmp > 9)
			tmp /= 10;
		str[*i] = '0' + tmp;
		(*i)++;
		return ;
	}
	copy_num_to_str(str, i, nbr / 10);
	copy_num_to_str(str, i, nbr % 10);
}

size_t	ft_sitoa(char *buff, long nbr)
{
	size_t	i;

	if (buff == NULL)
		return (0);
	i = 0;
	copy_num_to_str(buff, &i, nbr);
	return (i);
}
