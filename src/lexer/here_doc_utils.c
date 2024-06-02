/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:46:07 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 20:11:06 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	randomize_str(char *str, int rand_fd)
{
	unsigned char	c;
	size_t			len;

	len = ft_strlen(str);
	if (read(rand_fd, str, len) == -1)
	{
		perror("read");
		exit_with_code(NULL, EXIT_FAILURE);
	}
	while (str && *str)
	{
		c = *str;
		c = (c % 10) + '0';
		*str = c;
		str++;
	}
	return (1);
}

int	handle_here_doc_interrupt(int stdin_dup)
{
	g_shell.collecting_here_doc = FALSE;
	if (dup2(stdin_dup, STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit_with_code(NULL, EXIT_FAILURE);
	}
	close(stdin_dup);
	return (1);
}
