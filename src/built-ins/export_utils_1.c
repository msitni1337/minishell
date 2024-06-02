/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:44:41 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 09:44:41 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

char	*get_key_and_mode(char *arg, int *append)
{
	char	*tmp;

	*append = FALSE;
	tmp = ft_strchr(arg, '=');
	if (tmp > arg && *(tmp - 1) == '+')
	{
		*append = TRUE;
		tmp--;
	}
	return (ft_substr(arg, 0, tmp - arg));
}

void	print_env(t_cmd *cmd, t_lstenv *env)
{
	ft_putstr_fd("declare -x ", cmd->outfile);
	ft_putstr_fd(env->key, cmd->outfile);
	if (env->is_set)
	{
		ft_putstr_fd("=\"", cmd->outfile);
		ft_putstr_fd(env->value, cmd->outfile);
		ft_putchar_fd('"', cmd->outfile);
	}
	write(cmd->outfile, "\n", 1);
}

int	print_exported_env(t_cmd cmd)
{
	t_lstenv	**sorted_env;
	size_t		i;

	sorted_env = get_sorted_env();
	if (sorted_env == NULL)
		malloc_error(NULL, NULL, NULL, &cmd);
	i = 0;
	while (sorted_env && sorted_env[i])
	{
		if (ft_strcmp(sorted_env[i]->key, "_"))
			print_env(&cmd, sorted_env[i]);
		i++;
	}
	free(sorted_env);
	return (0);
}
