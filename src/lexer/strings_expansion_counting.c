/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_expansion_counting.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:11:11 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 10:22:33 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void count_var_len(t_string string, size_t *i, size_t *len)
{
	int count;
	char buff[BUFF_SZ];

	(*i)++;
	count = parse_key_count(string.s + *i);
	if (count > 0 && count < BUFF_SZ - 1)
	{
		ft_strlcpy(buff, string.s + *i, count + 1);
		if (ft_strcmp(buff, "?") == 0)
			*len += count_num_chars(g_shell.last_exit_value);
		else
			*len += ft_strlen(get_env_value(buff));
		*i += count;
	}
	else if (count == -1)
	{
		(*len)++;
	}
}

t_expansion_state count_normal_mode(t_string string, size_t *i, size_t *len,
									t_expansion_type expansion_type)
{
	t_expansion_state state;

	state = NORMAL_STATE;
	if (string.s[*i] == '\'')
	{
		state = SQUOTE_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(*i)++;
			return (state);
		}
	}
	else if (string.s[*i] == '"')
	{
		state = DQUOTE_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(*i)++;
			return (state);
		}
	}
	if (expansion_type & EXPAND_VARS && string.s[*i] == '$')
	{
		count_var_len(string, i, len);
	}
	else
	{
		(*i)++;
		(*len)++;
	}
	return (state);
}

t_expansion_state count_dquote_mode(t_string string, size_t *i, size_t *len,
									t_expansion_type expansion_type)
{
	t_expansion_state state;

	state = DQUOTE_STATE;
	if (string.s[*i] == '"')
	{
		state = NORMAL_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(*i)++;
			return (state);
		}
	}
	if (expansion_type & EXPAND_VARS && string.s[*i] == '$')
	{
		count_var_len(string, i, len);
	}
	else
	{
		(*i)++;
		(*len)++;
	}
	return (state);
}

t_expansion_state count_squote_mode(t_string string, size_t *i, size_t *len, t_expansion_type expansion_type)
{
	t_expansion_state state;

	state = SQUOTE_STATE;
	if (string.s[*i] == '\'')
	{
		state = NORMAL_STATE;
		if (expansion_type & REM_QUOTES)
		{
			(*i)++;
			return (state);
		}
	}
	(*i)++;
	(*len)++;
	return (state);
}

size_t get_expanded_str_len(t_string string, t_expansion_type expansion_type)
{
	t_expansion_state state;
	size_t len;
	size_t i;

	len = 0;
	i = 0;
	state = NORMAL_STATE;
	while (i < string.count)
	{
		if (state == NORMAL_STATE)
			state = count_normal_mode(string, &i, &len, expansion_type);
		else if (state == DQUOTE_STATE)
			state = count_dquote_mode(string, &i, &len, expansion_type);
		else if (state == SQUOTE_STATE)
			state = count_squote_mode(string, &i, &len, expansion_type);
	}
	return (len);
}
