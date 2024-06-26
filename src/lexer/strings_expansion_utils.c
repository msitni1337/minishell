/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:11:07 by msitni            #+#    #+#             */
/*   Updated: 2024/06/05 21:50:09 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

size_t	parse_key_count(const char *s)
{
	size_t	count;

	count = 0;
	if (s[count] == 0 || ft_isspace(s[count]))
		return (-1);
	if (ft_isdigit(s[count]))
		return (++count);
	else if (s[count] == '$' || s[count] == '?' || s[count] == '*')
		return (++count);
	while (s[count])
	{
		if (!ft_isalnum(s[count]) && s[count] != '_')
			break ;
		count++;
	}
	return (count);
}

int	count_num_chars(long n)
{
	int	i;

	i = 1;
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	return (i);
}

void	perform_var_value_copy(char *res, t_string *string, size_t *i,
		int key_count)
{
	char	key[BUFF_SZ];
	char	*value;

	ft_strlcpy(key, string->s, key_count + 1);
	if (ft_strcmp(key, "?") == 0)
		*i += ft_sitoa(res + *i, g_shell.last_exit_value);
	else
	{
		value = get_env_value(key);
		if (value)
			*i += ft_strlcpy(res + *i, value, INT32_MAX);
	}
	(string->count) -= key_count;
	(string->s) += key_count;
}

void	copy_var_value(char *res, t_string *string, size_t *i)
{
	int	key_count;

	(string->count)--;
	(string->s)++;
	key_count = parse_key_count(string->s);
	if (key_count > 0 && key_count < BUFF_SZ - 1)
	{
		perform_var_value_copy(res, string, i, key_count);
	}
	else if (key_count == -1)
	{
		res[*i] = '$';
		(*i)++;
	}
}

void	count_var_len(t_string string, size_t *i, size_t *len)
{
	int		count;
	char	buff[BUFF_SZ];

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
