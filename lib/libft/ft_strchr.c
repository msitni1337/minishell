/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:45:09 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:45:10 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			return (&(((char *)s)[i]));
		i++;
	}
	if (s && (!((unsigned char)c)))
		return (&(((char *)s)[i]));
	return (NULL);
}

/*
int	main(int c, char **v)
{
	char	*ptr1;
	char	*ptr2;

	(void)c;
	//char *ptr1 = ft_strchr(v[1], v[2][0]);
	ptr1 = ft_strchr(v[1], 1024);
	ptr2 = strchr(v[1], 1024);
	printf("%p\n", ptr1);
	printf("%p\n", ptr2);
}
*/
/*
 * ST-MED
 */
