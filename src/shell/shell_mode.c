/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_mode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:18:51 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:18:55 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	add_line_to_hist(char *line)
{
	t_lexer	lexer;
	t_token	token;

	lexer = new_lexer(line);
	token = get_next_token(&lexer, TRUE);
	if (token.type != TOKEN_EOF)
		add_history(line);
}

void	listen_for_cmd_line(void)
{
	char	*prompt;

	if (g_shell.line)
	{
		free(g_shell.line);
		g_shell.line = NULL;
	}
	prompt = get_prompt();
	g_shell.line = readline(prompt);
	free(prompt);
}

void	start_shell(void)
{
	listen_for_cmd_line();
	while (g_shell.line)
	{
		g_shell.interrupt = FALSE;
		add_line_to_hist(g_shell.line);
		if (parse_line(g_shell.line, &g_shell.tree_root) != NULL)
		{
			g_shell.last_exit_value = interpret_root(g_shell.tree_root);
		}
		else
		{
			g_shell.last_exit_value = 2;
			if (g_shell.interrupt == TRUE)
			{
				g_shell.interrupt = FALSE;
				close_here_docs();
				g_shell.last_exit_value = 130;
			}
		}
		free_tree(&g_shell.tree_root);
		listen_for_cmd_line();
	}
	ft_putendl_fd("exit", STDERR_FILENO);
}
