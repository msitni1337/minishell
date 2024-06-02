/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:19:46 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:27:48 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_shell.interrupt = TRUE;
	g_shell.last_exit_value = 130;
	if (g_shell.childs_pids.count > 0)
	{
		write(1, "\n", 1);
		return ;
	}
	if (g_shell.collecting_here_doc == TRUE)
	{
		write(1, "\n", 1);
		close(0);
		return ;
	}
	rl_on_new_line();
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
		exit(1);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		exit(1);
}
