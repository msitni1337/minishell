/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:43:08 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:43:12 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_fmemcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

static void	*ft_rmemcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = n;
	while (i--)
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (dest == src)
		return (dest);
	if (dest < src || src + n <= dest)
		return (ft_fmemcpy(dest, src, n));
	else
		return (ft_rmemcpy(dest, src, n));
}

/*
#include <string.h>

void	check_segv(int i)
{
	printf("Reached here: %d\n", i);
	fflush(0);
}

int	main(int c, char**v)
{
		ft_memmove("dsds", 0, 5);
		check_segv(0);
		memmove("dsds",0,5);
		check_segv(1);
}
*/
/*
 * ST-MED
 */
