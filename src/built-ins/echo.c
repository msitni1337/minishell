/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:22:43 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/01 11:50:51 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

bool check_flags(char** argv, int*i)
{
	bool print_nl;
	int j;

	print_nl = TRUE;
	while (argv[*i] && argv[*i][0] == '-')
	{
		j = 1;
		while (argv[*i][j] == 'n')
			j++;
		if (argv[*i][j] == '\0')
		{
			(*i)++;
			print_nl = FALSE;
		}
		else
			break ;
	}
	return print_nl;
}

int	check_echo_options(t_cmd cmd)
{
	bool print_nl;
	int	i;

	i = 1;
	print_nl = check_flags(cmd.argv, &i);
	while (cmd.argv[i])
	{
		write(cmd.outfile, cmd.argv[i], ft_strlen(cmd.argv[i]));
		if (cmd.argv[i + 1])
			write(cmd.outfile, " ", 1);
		i++;
	}
	if (print_nl == TRUE)
		write(cmd.outfile, "\n", 1);
	return 0;
}

int ft_echo(t_cmd cmd)
{
	if (cmd.argc == 1)
	{
		write(cmd.outfile, "\n", 1);
		return errno;
	}
	return check_echo_options(cmd);
}

// int main(int ac, char **av)
// {
// 	ft_echo(ac, av);
// 	return (0);
// }
