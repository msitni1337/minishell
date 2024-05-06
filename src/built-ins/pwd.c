/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:10:38 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/23 16:58:19 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

int		ft_pwd()
{
	char path[PATH_MAX];

	if (getcwd(path, sizeof(path)) != NULL)
	{
		ft_putendl_fd(path, 1);
	}
	else
	{
		perror("minishell: pwd");
		return errno;
	}
	return 0;
}

// int main(int ac, char **av, const char **envp)
// {
// 	ft_pwd(ac, av);
// 	return (0);
// }