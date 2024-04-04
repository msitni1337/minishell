/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:47:19 by msitni            #+#    #+#             */
/*   Updated: 2023/11/23 22:41:21 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t sz)
{
	size_t	i;

	i = 0;
	if (!sz)
		return (ft_strlen(src));
	while (src[i] && i < sz - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (ft_strlen(src));
}

/*
//#include <bsd/string.h>
#include <string.h>

int	main(int c, char**v)
{
	size_t	x1;
	size_t	x2;

	(void) c;
	char* src1 = strdup("Hello World");
	char* dest1 = malloc(1000);
	char* src2  = strdup("Hello World");
	char* dest2 = malloc(1000);
	x1 = strlcpy(NULL, src1, 10);
	//x2 = ft_strlcpy(NULL, src1, 10);
	printf("%ld\n", x1);
	return (0);
}
*/
/*
 * ST-MED
 */
