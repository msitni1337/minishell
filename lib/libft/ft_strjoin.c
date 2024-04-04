/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 22:41:34 by msitni            #+#    #+#             */
/*   Updated: 2023/11/23 22:41:35 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		s1_len;
	int		s2_len;

	if (!s1 || !s2)
	{
		if (s1)
			return (ft_strdup(s1));
		if (s2)
			return (ft_strdup(s2));
		return (NULL);
	}
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	str = malloc(s1_len + s2_len + 1);
	if (!str)
		return (NULL);
	str[0] = 0;
	ft_strlcat(str, s1, s1_len + s2_len + 1);
	ft_strlcat(str, s2, s1_len + s2_len + 1);
	return (str);
}

/*
int	main(int c, char**v)
{
	//printf("%s\n", ft_strjoin(v[1], v[2]));
	//printf("Passing NULL args to func: %s\n", ft_strjoin(0, 0));
}
*/
/*
 * ST-MED
 */
