/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:11:15 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 10:21:29 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_expansion_state expand_normal_mode(char *res, t_string *string, size_t *i,
									 t_expansion_type expansion_type)
{
	t_expansion_state state;

	state = NORMAL_STATE;
	if (*(string->s) == '\'')
	{
		state = SQUOTE_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(string->count)--;
			(string->s)++;
			return (state);
		}
	}
	else if (*(string->s) == '"')
	{
		state = DQUOTE_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(string->count)--;
			(string->s)++;
			return (state);
		}
	}
	if (expansion_type & EXPAND_VARS && *(string->s) == '$')
	{
		copy_var_value(res, string, i);
	}
	else
	{
		res[*i] = *(string->s);
		(string->count)--;
		(string->s)++;
		(*i)++;
	}
	return (state);
}

t_expansion_state expand_dquote_mode(char *res, t_string *string, size_t *i,
									 t_expansion_type expansion_type)
{
	t_expansion_state state;

	state = DQUOTE_STATE;
	if (*(string->s) == '"')
	{
		state = NORMAL_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(string->count)--;
			(string->s)++;
			return (state);
		}
	}
	if (expansion_type & EXPAND_VARS && *(string->s) == '$')
	{
		copy_var_value(res, string, i);
	}
	else
	{
		res[*i] = *(string->s);
		(string->count)--;
		(string->s)++;
		(*i)++;
	}
	return (state);
}

t_expansion_state expand_squote_mode(char *res, t_string *string, size_t *i,
									 t_expansion_type expansion_type)
{
	t_expansion_state state;

	state = SQUOTE_STATE;
	if (*(string->s) == '\'')
	{
		state = NORMAL_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(string->count)--;
			(string->s)++;
			return (state);
		}
	}
	res[*i] = *(string->s);
	(string->count)--;
	(string->s)++;
	(*i)++;
	return (state);
}

char *perform_string_expansion(t_string string, t_expansion_type expansion_type)
{
	t_expansion_state state;
	char *res;
	size_t len;
	size_t i;

	len = get_expanded_str_len(string, expansion_type);
	res = malloc(len + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	state = NORMAL_STATE;
	while (string.count > 0)
	{
		if (state == NORMAL_STATE)
			state = expand_normal_mode(res, &string, &i, expansion_type);
		else if (state == DQUOTE_STATE)
			state = expand_dquote_mode(res, &string, &i, expansion_type);
		else if (state == SQUOTE_STATE)
			state = expand_squote_mode(res, &string, &i, expansion_type);
	}
	res[i] = 0;
	return (res);
}

char *expand_string(t_string string, t_expansion_type expansion_type)
{
	if (contains_chars(string, "*'\"$") == TRUE)
		return (perform_string_expansion(string, expansion_type));
	else
		return (ft_substr(string.s, 0, string.count));
}
