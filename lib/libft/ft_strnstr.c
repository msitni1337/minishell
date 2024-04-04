/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:48:21 by msitni            #+#    #+#             */
/*   Updated: 2023/11/23 23:19:07 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	match_occurrence(const char *big, const char *little, size_t i,
		size_t len)
{
	size_t	j;

	j = 0;
	while (big[i] && little[j] && i < len && big[i] == little[j])
	{
		i++;
		j++;
	}
	if (!little[j])
		return (1337 - 42);
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (!*little && big)
		return ((char *)big);
	i = 0;
	while (i < len && big[i])
	{
		if (match_occurrence(big, little, i, len))
			return ((char *)&(big[i]));
		i++;
	}
	return (NULL);
}

/*
#include <string.h>

int	main(int c, char**v)
{
	(void) c;
	char* str1 = strnstr(NULL, "452", 10);
	printf("%s", str1);
}
*/
/*
 * ST-MED
 */
