/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:48:42 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/24 17:50:53 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

void	ft_env(t_shell *shell, char **av, const char **envp)
{
	USED(av);
	init_shell(shell);
	take_env(&shell->env_list, envp);
	print_list(shell->env_list);
}

// int main(int ac, char **av, const char **envp)
// {
// 	t_shell shell;


// 	USED(ac);
// 	ft_env(&shell, av, envp);
// 	return (0);
// }