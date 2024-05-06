/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:22:43 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/21 16:54:14 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

void	check_echo_options(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i] && av[i][0] == '-')
	{
		j = 1;
		while (av[i][j] == 'n')
			j++;
		if (av[i][j] == '\0')
			i++;
		else
			break ;
	}
	while (av[i])
	{
		write(1, av[i], ft_strlen(av[i]));
		if (av[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (av[1] && av[1][0] != '-')
		write(1, "\n", 1);
}

int ft_echo(int ac, char **av)
{
	if (ac == 1)
	{
		write(1, "\n", 1);
		return ;
	}
	check_echo_options(av);
}

// int main(int ac, char **av)
// {
// 	ft_echo(ac, av);
// 	return (0);
// }
