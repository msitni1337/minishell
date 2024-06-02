/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:25:43 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 20:11:31 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared.h"

void	*free_arr(char **arr)
{
	char	**tmp;

	tmp = arr;
	while (arr && *arr)
	{
		free(*arr);
		arr++;
	}
	if (arr)
		free(tmp);
	return (NULL);
}

void	close_here_docs(void)
{
	struct stat	s;
	int			fd;

	fd = 3;
	while (fd > 2 && fstat(fd, &s) != -1)
	{
		close(fd);
		fd++;
	}
}

void	free_cmd(t_cmd *cmd)
{
	char	*argv0;

	if (cmd == NULL)
		return ;
	argv0 = NULL;
	if (cmd->argv)
	{
		argv0 = cmd->argv[0];
		free_arr(cmd->argv);
	}
	if (cmd->bin_path && argv0 != cmd->bin_path)
		free(cmd->bin_path);
	cmd->argv = NULL;
	cmd->bin_path = NULL;
}
