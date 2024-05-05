/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:08:00 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/24 17:38:16 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

void	ft_unset(t_shell *shell, char **av)
{
	int		i;
	char	*key;

	i = 1;
	while (av[i])
	{
		key = find_env_key(shell->env_list, av[i]);
		if (key)
			remove_env(&shell->env_list, key);
		i++;
	}
}

// int main(int ac, char **av, const char **envp)
// {
// 	t_shell shell;

// 	USED(ac);
// 	init_shell(&shell);
// 	take_env(&shell.env_list, envp);
// 	printf("before unset\n");
// 	printf("%s\n", get_env_value(shell.env_list, "HOME"));
// 	ft_unset(&shell, av);
// 	char *s = get_env_value(shell.env_list, "HOME");
// 	if (!s)
// 		printf("HOME is unset\n");
// 	else
// 		printf("HOME is set\n");
// 	return (0);
// }