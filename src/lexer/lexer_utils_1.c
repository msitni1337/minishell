/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:06:31 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 10:57:47 by msitni           ###   ########.fr       */
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
