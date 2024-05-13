/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:48:09 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:48:11 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	if (s1 == s2)
		return (0);
	if (s1 == NULL || s2 == NULL)
		return -1;
	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (n == 0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*
#include <string.h>

int	main(int c, char**v)
{
	char	*ptr;
	char	*ptr0;

	char	ptr1[] = "dfs";
	(void) c;
	(void) v;
	//printf("%d\n", ft_strncmp(v[1], v[2], atoi(v[3])));
	//printf("%d\n", ft_strncmp("test\200", "test\0", 6));
	//return (0);
	ptr = 0;
	ptr0 = 0;
	printf("%d\n", strncmp(0, ptr1, 10));
	fflush(0);
	printf("%d\n", ft_strncmp(0, ptr1, 10));
	// -> Segfault here.
	//printf("%d\n", ft_strncmp("Hello", "World", 10));
	//printf("%d\n", strncmp(ptr, ptr, 10)); // -> But not here here.
	//printf("%d\n", strncmp(ptr0, ptr, 10)); // -> Segfault here.
}
*/
/*
 * ST-MED
 */
