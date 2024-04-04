/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:42:00 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:42:03 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*str1;
	const unsigned char	*str2;

	if (s1 == s2)
		return (0);
	str1 = s1;
	str2 = s2;
	i = 0;
	while (i < n && str1[i] == str2[i])
		i++;
	if (i == n)
		return (0);
	return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
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
	(void) c;
	printf("%d\n", ft_memcmp("abc2", "abc1",3));
	check_segv(0);
	printf("%d\n", memcmp("abc1", "abc2",3));
	check_segv(1);
}
*/
/*
 * ST-MED
 */
