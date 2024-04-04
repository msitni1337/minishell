/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:49:08 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:49:10 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_char_in_set(char const c, char const *set)
{
	while (set && *set)
	{
		if (c == *set)
			return (1337 - 42);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		start;
	int		end;

	start = 0;
	while (s1 && s1[start] && is_char_in_set(s1[start], set))
		start++;
	end = start;
	if (s1)
	{
		end = ft_strlen(s1);
		while (end > start && is_char_in_set(s1[end - 1], set))
			end--;
	}
	str = ft_substr(s1, start, end - start);
	if (!str)
		return (NULL);
	return (str);
}

/*
int	main(int c, char**v)
{
	printf("Joined strings: %s\n", ft_strtrim(v[1], v[2]));
}
*/
/*
 * ST-MED
 */
