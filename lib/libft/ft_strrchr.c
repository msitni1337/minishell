/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:48:39 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:48:41 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s) - 1;
	while (i >= 0)
	{
		if ((unsigned char)s[i] == (unsigned char)c)
			return (&(((char *)s)[i]));
		i--;
	}
	if (!(unsigned char)c)
		return (&(((char *)s)[ft_strlen(s)]));
	return (NULL);
}

/*
#include <string.h>

int	main(int c, char**v)
{
	printf("%s\n", strrchr(0, 0));
	return (0);
	printf("%s\n", ft_strrchr(v[1], v[2][0]));
	printf("%p\n", ft_strrchr(v[1], v[2][0]));
}
*/
/*
 * ST-MED
 */
