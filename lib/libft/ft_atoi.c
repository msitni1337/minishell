/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:34:12 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:34:17 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n'
		|| c == '\f');
}

int	ft_atoi(const char *nptr)
{
	unsigned long	result;
	int				sign;
	int				i;

	sign = 1;
	i = 0;
	while (is_space(nptr[i]))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	result = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + nptr[i] - '0';
		if (sign > 0 && result > (SIZE_MAX / 2) - 1)
			return (-1);
		if (sign < 0 && result > (SIZE_MAX / 2) - 1)
			return (0);
		i++;
	}
	return (result * sign);
}

/*
int	main(int c, char**v)
{
	(void) c;
	printf("%d\n", ft_atoi(v[1]));
	printf("%d\n", atoi(v[1]));
}
*/
/*
 * ST-MED
 */
