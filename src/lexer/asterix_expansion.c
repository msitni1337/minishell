/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterix_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:41:03 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 10:41:27 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	matching_loop(char **pattern, char *name, char *star, char *star_pos)
{
	while (*name)
	{
		if (**pattern == *name)
		{
			name++;
			(*pattern)++;
			continue ;
		}
		if (**pattern == '*')
		{
			star = (*pattern)++;
			star_pos = name;
			continue ;
		}
		if (star)
		{
			(*pattern) = star + 1;
			name = ++star_pos;
			continue ;
		}
		return (FALSE);
	}
	return (TRUE);
}

char	**add_matching_filenames(char **res, size_t *index, char *argv,
		char **cwdfiles)
{
	int		has_match;
	size_t	i;

	i = 0;
	has_match = FALSE;
	while (cwdfiles[i])
	{
		if (is_pattern_matching(argv, cwdfiles[i]))
		{
			res[*index] = ft_strdup(cwdfiles[i]);
			if (res[*index] == NULL)
			{
				free_arr(cwdfiles);
				free_arr(res);
				return (NULL);
			}
			has_match = TRUE;
			(*index)++;
		}
		i++;
	}
	if (has_match == FALSE)
		return (copy_same_argument(res, index, argv, cwdfiles));
	return (res);
}

char	**init_asterices_expansion_arr(char ***cwdfiles_p, char **argv,
		size_t *count)
{
	char	**res;

	*cwdfiles_p = get_all_cwd_filenames();
	if (*cwdfiles_p == NULL)
		return (free_arr(argv));
	ft_sort_char_arr(*cwdfiles_p);
	*count = get_expanded_args_count(argv, *cwdfiles_p);
	res = malloc(sizeof(char *) * (*count + 1));
	if (res == NULL)
	{
		free_arr(*cwdfiles_p);
		return (free_arr(argv));
	}
	res[*count] = NULL;
	return (res);
}

char	**asterice_expansion_loop(char **res, char **argv, size_t argc,
		char **cwdfiles)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < argc)
	{
		if (ft_strchr(argv[i], '*'))
		{
			if (add_matching_filenames(res, &j, argv[i], cwdfiles) == NULL)
				return (free_arr(argv));
		}
		else
		{
			if (copy_same_argument(res, &j, argv[i], cwdfiles) == NULL)
				return (free_arr(argv));
		}
		i++;
	}
	return (res);
}

char	**expand_asterices(char **argv, size_t *argc)
{
	char	**cwdfiles;
	char	**res;
	size_t	count;

	res = init_asterices_expansion_arr(&cwdfiles, argv, &count);
	if (res == NULL)
		return (NULL);
	if (asterice_expansion_loop(res, argv, *argc, cwdfiles) == NULL)
		return (NULL);
	free_arr(argv);
	free_arr(cwdfiles);
	*argc = count;
	return (res);
}
