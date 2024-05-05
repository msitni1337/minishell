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

void	ft_pwd(int ac, char **av)
{
	char path[PATH_MAX];

	USED(ac);
	USED(av);
	if (getcwd(path, sizeof(path)) != NULL)
	{
		ft_putendl_fd(path, 1);
	}
	else
	{
		perror("minishell: pwd");
		return ;
	}
}

// int main(int ac, char **av, const char **envp)
// {
// 	ft_pwd(ac, av);
// 	return (0);
// }