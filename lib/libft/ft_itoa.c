/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 22:33:54 by msitni            #+#    #+#             */
/*   Updated: 2023/11/23 22:38:08 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_chars(long n)
{
	int	i;

	i = 1;
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static int	get_first_digit(long n)
{
	while (n > 9)
	{
		n /= 10;
	}
	return ((int)n);
}

static void	fill_str_with_nbr(long nbr, char *str, int *i)
{
	if (nbr < 0)
	{
		nbr = -nbr;
		str[*i] = '-';
		(*i)++;
	}
	if (nbr >= 0 && nbr <= 9)
	{
		str[*i] = '0' + get_first_digit(nbr);
		(*i)++;
		return ;
	}
	fill_str_with_nbr(nbr / 10, str, i);
	fill_str_with_nbr(nbr % 10, str, i);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;

	str = malloc(count_chars(n) + 1);
	if (!str)
		return (NULL);
	i = 0;
	fill_str_with_nbr(n, str, &i);
	str[i] = 0;
	return (str);
}

/*
#include <stdlib.h>

int	main(int c, char**v)
{
	printf("%s\n", itoa(atoi(v[1])));
}
*/
/*
 * ST-MED
 */
