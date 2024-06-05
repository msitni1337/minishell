/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:03:20 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:03:45 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	get_string_whitespace(t_lexer *lexer, t_string *s)
{
	while (lexer->pos < lexer->count)
	{
		if (ft_isspace(lexer->line[lexer->pos])
			|| lexer->line[lexer->pos] == DQUOTE
			|| lexer->line[lexer->pos] == SQUOTE
			|| is_special(lexer->line[lexer->pos]))
			break ;
		s->count++;
		lexer->pos++;
	}
	return (TRUE);
}

int	get_string_delim(t_lexer *lexer, t_string *s, const char delim)
{
	int	num_of_delim;

	num_of_delim = 0;
	while (lexer->pos < lexer->count && num_of_delim < 2)
	{
		if (lexer->line[lexer->pos] == delim)
			num_of_delim++;
		s->count++;
		lexer->pos++;
	}
	if (num_of_delim == 2)
		return (TRUE);
	return (FALSE);
}

int	get_string(t_lexer *lexer, t_string *s)
{
	int	is_closed;

	s->count = 0;
	s->s = lexer->line + lexer->pos;
	if (*(s->s) == DQUOTE)
		is_closed = get_string_delim(lexer, s, DQUOTE);
	else if (*(s->s) == SQUOTE)
		is_closed = get_string_delim(lexer, s, SQUOTE);
	else
		is_closed = get_string_whitespace(lexer, s);
	while (is_closed == TRUE && s->s[s->count] && !ft_isspace(s->s[s->count])
		&& !is_special(s->s[s->count]))
	{
		if (s->s[s->count] == DQUOTE)
			is_closed = get_string_delim(lexer, s, DQUOTE);
		else if (s->s[s->count] == SQUOTE)
			is_closed = get_string_delim(lexer, s, SQUOTE);
		else
			is_closed = get_string_whitespace(lexer, s);
	}
	return (is_closed);
}

t_node	*add_str_node(t_node *root, t_lexer *lexer)
{
	t_node	*node;

	lexer->pos--;
	node = create_node(NODE_STR);
	if (node == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	if (get_string(lexer, &(node->token_str)) == FALSE)
	{
		free(node);
		return (NULL);
	}
	append_child(root, node);
	return (node);
}
