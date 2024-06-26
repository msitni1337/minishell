/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_mode.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:13:19 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:13:23 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

char	*read_entire_stdin(void)
{
	char	buff[BUFF_SZ];
	char	*res;
	char	*tmp;
	int		bytes;

	res = NULL;
	bytes = read(STDIN_FILENO, buff, BUFF_SZ - 1);
	while (bytes > 0)
	{
		buff[bytes] = 0;
		tmp = ft_strjoin(res, buff);
		if (tmp == NULL)
			malloc_error(res, NULL, NULL, NULL);
		free(res);
		res = tmp;
		bytes = read(STDIN_FILENO, buff, BUFF_SZ - 1);
	}
	return (res);
}

void	execute_from_stdin(void)
{
	g_shell.line = read_entire_stdin();
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
	exit_with_code(NULL, g_shell.last_exit_value);
}
