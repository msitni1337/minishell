/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 21:58:29 by msitni            #+#    #+#             */
/*   Updated: 2024/06/05 21:59:08 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	**expand_argument(t_darr *result, char *arg)
{
	t_lexer		lexer;
	t_string	str;
	char		*s;

	lexer = new_lexer(arg);
	trim_spaces(&lexer);
	while (lexer.pos < lexer.count)
	{
		get_string(&lexer, &str, NULL);
		s = expand_string(str, REM_QUOTES);
		if (s == NULL)
			return (NULL);
		if (add_to_arr(result, &s) == NULL)
		{
			free(s);
			return (NULL);
		}
		trim_spaces(&lexer);
	}
	return (result->data);
}

char	**expand_args(char **args, size_t *count)
{
	t_darr	result;
	size_t	i;

	result = init_da(sizeof(char *));
	result.data = malloc(sizeof(char *));
	((char **)(result.data))[0] = NULL;
	result.capacity = 1;
	i = 0;
	while (i < *count)
	{
		if (expand_argument(&result, args[i]) == NULL)
		{
			free_arr(args);
			free_arr(result.data);
			return (NULL);
		}
		i++;
	}
	free_arr(args);
	*count = result.count;
	return (result.data);
}
