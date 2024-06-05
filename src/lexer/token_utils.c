/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:06:49 by msitni            #+#    #+#             */
/*   Updated: 2024/06/05 20:54:02 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lexer	new_lexer(char *line)
{
	t_lexer	lexer;

	lexer.line = line;
	lexer.count = ft_strlen(line);
	lexer.pos = 0;
	return (lexer);
}

void	trim_spaces(t_lexer *lexer)
{
	while (lexer->pos < lexer->count && ft_isspace(lexer->line[lexer->pos]))
		lexer->pos++;
}

int	is_special(char c)
{
	return (c == '(' || c == ')' || c == '<' || c == '>' || c == '|');
}
