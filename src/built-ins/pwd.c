/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:10:38 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/01 11:50:51 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

int		ft_pwd(t_cmd cmd)
{
	char *path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("get_cwd");
		exit_with_code(&cmd, EXIT_FAILURE);
	}
	ft_putendl_fd(path, cmd.outfile);
	free(path);
	return 0;
}

// int main(int ac, char **av, const char **envp)
// {
// 	ft_pwd(ac, av);
// 	return (0);
// }