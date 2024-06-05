/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:06:31 by msitni            #+#    #+#             */
/*   Updated: 2024/06/05 21:52:56 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	is_cmd_token(t_token token)
{
	return ((token).type == TOKEN_STR || (token).type == TOKEN_REDIRECT_IN
		|| (token).type == TOKEN_REDIRECT_OUT || (token).type == TOKEN_HERE_DOC
		|| (token).type == TOKEN_APPEND || (token).type == TOKEN_OPEN_PAREN);
}

int	is_redirect_token(t_token token)
{
	return ((token).type == TOKEN_REDIRECT_IN
		|| (token).type == TOKEN_REDIRECT_OUT || (token).type == TOKEN_HERE_DOC
		|| (token).type == TOKEN_APPEND);
}

int	is_logic_op(t_token token)
{
	return ((token).type == TOKEN_PIPE || (token).type == TOKEN_AND
		|| (token).type == TOKEN_OR);
}

int	contains_chars(t_string string, char *charset)
{
	size_t	i;
	size_t	j;
	size_t	charset_len;

	charset_len = ft_strlen(charset);
	i = 0;
	while (i < string.count)
	{
		j = 0;
		while (j < charset_len)
		{
			if (string.s[i] == charset[j])
				return (TRUE);
			j++;
		}
		i++;
	}
	return (FALSE);
}

int	write_next_line_here_doc(t_node *node, int write)
{
	char	*line;
	char	*delim;

	g_shell.collecting_here_doc = TRUE;
	delim = expand_string(node->token_str, REM_QUOTES);
	if (delim == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	line = readline("> ");
	while (line)
	{
		if (ft_strcmp(line, delim) == 0)
			break ;
		ft_putendl_fd(line, write);
		free(line);
		line = readline("> ");
	}
	close(write);
	free(delim);
	if (line)
	{
		free(line);
		return (0);
	}
	return (1);
}
