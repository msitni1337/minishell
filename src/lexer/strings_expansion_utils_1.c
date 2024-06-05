/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_expansion_utils_1.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 21:58:16 by msitni            #+#    #+#             */
/*   Updated: 2024/06/05 22:03:24 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	*expand_string(t_string string, t_expansion_type expansion_type)
{
	if (contains_chars(string, "*'\"$") == TRUE)
		return (perform_string_expansion(string, expansion_type));
	else
		return (ft_substr(string.s, 0, string.count));
}
