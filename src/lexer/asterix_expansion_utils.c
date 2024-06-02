/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterix_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:42:04 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 10:42:04 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	**get_all_cwd_filenames(void)
{
	char			*tmp;
	DIR				*cwdir;
	struct dirent	*dir;
	t_darr			res;

	res = init_da(sizeof(char *));
	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
		return (NULL);
	cwdir = opendir(tmp);
	free(tmp);
	if (cwdir == NULL)
		return (NULL);
	dir = readdir(cwdir);
	while (dir)
	{
		tmp = ft_strdup(dir->d_name);
		if (tmp == NULL)
			return (free_p(NULL, NULL, NULL, res.data));
		if (add_to_arr(&res, &tmp) == NULL)
			return (free_p(tmp, NULL, NULL, res.data));
		dir = readdir(cwdir);
	}
	closedir(cwdir);
	return (res.data);
}

int	is_pattern_matching(char *pattern, char *name)
{
	if (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0)
		return (FALSE);
	if (*pattern != '.' && *name == '.')
		return (FALSE);
	if (matching_loop(&pattern, name, NULL, name) == FALSE)
		return (FALSE);
	while (*pattern == '*')
		pattern++;
	return (*pattern == 0);
}

char	**copy_same_argument(char **res, size_t *index, char *argv,
		char **cwdfiles)
{
	res[*index] = ft_strdup(argv);
	if (res[*index] == NULL)
	{
		free_arr(cwdfiles);
		free_arr(res);
		return (NULL);
	}
	(*index)++;
	return (res);
}

void	count_matching(char *argv, char **cwdfiles, size_t *res)
{
	int	has_match;
	int	i;

	has_match = FALSE;
	i = 0;
	while (cwdfiles[i])
	{
		if (is_pattern_matching(argv, cwdfiles[i]))
		{
			(*res)++;
			has_match = TRUE;
		}
		i++;
	}
	if (has_match == FALSE)
		(*res)++;
}

size_t	get_expanded_args_count(char **argv, char **cwdfiles)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (argv && argv[i])
	{
		if (ft_strchr(argv[i], '*'))
			count_matching(argv[i], cwdfiles, &count);
		else
			count++;
		i++;
	}
	return (count);
}
