/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 22:42:31 by msitni            #+#    #+#             */
/*   Updated: 2023/11/23 22:42:32 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	s_len;
	size_t	i;

	if (!s || !f)
	{
		if (!s)
			return (ft_strdup(""));
		if (!f)
			return (ft_strdup(s));
	}
	s_len = ft_strlen(s);
	str = malloc(s_len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = 0;
	return (str);
}

/*
char	capitalize_depending_on_parity(unsigned int index, char c)
{
	if (index % 2 && c >= 'A' && c <= 'Z')
		return (c + 32);
	if (index % 2 == 0 && c >= 'a' && c < 'z')
		return (c - 32);
	return (c);
}

int	main(int c, char**v)
{
	printf("%s\n", ft_strmapi(v[1], &capitalize_depending_on_parity));
}
*/
/*
 * ST-MED
 */
