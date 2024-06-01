/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:06:01 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/01 11:57:57 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

t_lstenv	*add_env(char *key, char *arg, int append)
{
	char		*value;
	char		*tmp;
	t_lstenv	*node;

	value = ft_strdup(ft_strchr(arg, '=') + 1);
	if (value == NULL)
		return (NULL);
	if (append == TRUE)
	{
		node = add_or_replace_env(key, NULL);
		if (node == NULL)
			return (free_p(value, NULL, NULL, NULL));
		node->is_set = TRUE;
		tmp = ft_strjoin(node->value, value);
		free_p(node->value, NULL, NULL, NULL);
		if (tmp == NULL)
			return (free_p(value, NULL, NULL, NULL));
		node->value = tmp;
	}
	else
	{
		node = add_or_replace_env(key, value);
	}
	free(value);
	return (node);
}

void	export_env_to_list(t_cmd *cmd, char *arg, char *key, int append)
{
	t_lstenv	*node;

	if (ft_strchr(arg, '='))
		node = add_env(key, arg, append);
	else
		node = add_or_replace_env(key, NULL);
	if (node == NULL)
		malloc_error(key, NULL, NULL, cmd);
}

int	export_env(t_cmd *cmd, size_t i)
{
	int		ret_value;
	int		append;
	char	*key;

	key = get_key_and_mode(cmd->argv[i], &append);
	if (key == NULL)
		malloc_error(NULL, NULL, NULL, cmd);
	ret_value = 0;
	if (ft_strlen(key) && check_key_is_valid(key) == 0)
		export_env_to_list(cmd, cmd->argv[i], key, append);
	else
		ret_value = key_not_valid("export", key);
	free(key);
	return (ret_value);
}

int	parse_export(t_cmd cmd)
{
	int		tmp;
	int		ret_value;
	size_t	i;

	i = 1;
	ret_value = 0;
	while (i < cmd.argc)
	{
		tmp = export_env(&cmd, i);
		if (ret_value == 0)
			ret_value = tmp;
		i++;
	}
	return (ret_value);
}

int	ft_export(t_cmd cmd)
{
	if (cmd.argc == 1)
		return (print_exported_env(cmd));
	return (parse_export(cmd));
}
