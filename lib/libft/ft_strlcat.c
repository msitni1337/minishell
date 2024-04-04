/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 22:41:47 by msitni            #+#    #+#             */
/*   Updated: 2023/11/23 22:41:49 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	st_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

static size_t	ft_min_s(size_t a, size_t b)
{
	if (a > b)
		return (b);
	return (a);
}

size_t	ft_strlcat(char *dst, const char *src, size_t sz)
{
	size_t	total_len;
	size_t	dst_len;
	size_t	i;

	dst_len = st_strlen(dst);
	total_len = ft_min_s(sz, dst_len) + ft_strlen(src);
	i = 0;
	if (!sz)
		return (total_len);
	while (dst && src && src[i] && dst_len + i < sz - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = 0;
	i++;
	return (total_len);
}

/*
//#include <bsd/string.h>
#include <string.h>

void	check_segv(int i)
{
	printf("Reached here: %d\n", i);
	fflush(0);
}

int	main(int c, char**v)
{
	char	*dst1;
	char	*dst2;
	size_t	x1;
	size_t	x2;
	char	*src2;

	//char	*src1 = malloc(100);
	char* src1 = strdup("hello");
	src2 = malloc(100);
	x1 = strlcat(0, src1, 20);
	check_segv(x1);
	x2 = ft_strlcat(0, src1, 20);
	check_segv(x2);
	return (0);
}
*/
/*
 * ST-MED
 */
