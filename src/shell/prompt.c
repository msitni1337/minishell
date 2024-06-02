/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:19:09 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:20:40 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*get_chopped_prompt(char *cwd, int home_len)
{
	char	*res;
	char	*tmp;

	tmp = ft_strjoin(PROMPTSTART "~", cwd + home_len);
	free(cwd);
	if (tmp == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	res = ft_strjoin(tmp, PROMPTEND);
	free(tmp);
	if (res == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	return (res);
}

char	*get_full_prompt(char *cwd)
{
	char	*res;
	char	*tmp;

	tmp = ft_strjoin(PROMPTSTART, cwd);
	free(cwd);
	if (tmp == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	res = ft_strjoin(tmp, PROMPTEND);
	free(tmp);
	if (res == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	return (res);
}

char	*get_prompt(void)
{
	char	*home;
	char	*cwd;
	int		home_len;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("get_cwd");
		exit_with_code(NULL, EXIT_FAILURE);
	}
	home = get_env_value("HOME");
	home_len = ft_strlen(home);
	if (home && ft_strncmp(cwd, home, home_len) == 0)
		return (get_chopped_prompt(cwd, home_len));
	else
		return (get_full_prompt(cwd));
}
