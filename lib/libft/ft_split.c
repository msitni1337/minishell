/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:44:55 by msitni            #+#    #+#             */
/*   Updated: 2023/11/22 00:44:57 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**handle_error(char **ptr, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (NULL);
}

static int	count_strs(char const *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
		{
			while (str[i] && str[i] == c)
				i++;
		}
		else
		{
			count++;
			while (str[i] && str[i] != c)
				i++;
		}
	}
	return (count);
}

static size_t	get_next_str_size(char const *str, char c)
{
	size_t	i;

	i = 0;
	while (str && str[i] && str[i] != c)
		i++;
	return (i);
}

static char	**perform_split(char **res, char const *s, char c)
{
	int		index;
	int		curr_index;
	char	*str;
	size_t	str_s;

	curr_index = 0;
	index = 0;
	while (s && s[curr_index])
	{
		str_s = get_next_str_size(s + curr_index, c);
		if (!str_s)
			curr_index++;
		else
		{
			str = malloc(str_s + 1);
			if (!str)
				return (handle_error(res, index));
			ft_strlcpy(str, s + curr_index, str_s + 1);
			res[index] = str;
			index++;
			curr_index += str_s;
		}
	}
	res[index] = 0;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char **) * (count_strs(s, c) + 1));
	if (!res)
		return (NULL);
	return (perform_split(res, s, c));
}

/*
int	main(int c, char**v)
{
	(void) c;
	char**set = ft_split(v[1], v[2][0]);
	printf("Number Of Elements: %d\n", count_strs(v[1], v[2][0]
#ifdef FirstMethod
, NULL
#endif
));
	printf("==========Chopped Strs:\n");
	while (*set)
	{
		printf("=> %s\n", *set);
		set++;
	}
	printf("=======================\n");
}
*/
/*
 * ST-MED
 */
