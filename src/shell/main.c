/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:14:00 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:21:23 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_shell	g_shell;

int	main(int c, const char **v, const char **env)
{
	(void)c;
	(void)v;
	init_shell(env);
	if (isatty(0) == 1)
		start_shell();
	else
		execute_file();
	return (g_shell.last_exit_value);
}
