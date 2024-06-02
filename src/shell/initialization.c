/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:12:44 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:12:46 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "shell.h"

void	init_shell(const char **envp)
{
	setup_signal_handlers();
	g_shell.last_exit_value = 0;
	g_shell.childs_pids = init_da(sizeof(int));
	take_env(envp);
	g_shell.interrupt = FALSE;
	g_shell.collecting_here_doc = FALSE;
}
