/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:33:59 by msitni            #+#    #+#             */
/*   Updated: 2024/06/05 20:41:10 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"
#include "shared.h"

void	print_error(char *name, char *reason)
{
	ft_putstr_fd(PROG_NAME ": ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(reason, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
}

void	print_error_str(t_string str, char *reason)
{
	ft_putstr_fd(PROG_NAME ": ", STDERR_FILENO);
	write(STDERR_FILENO, str.s, str.count);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(reason, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
}

void	*syntax_error(char *reason)
{
	ft_putstr_fd(PROG_NAME ": ", STDERR_FILENO);
	ft_putstr_fd(reason, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	return (NULL);
}

void	malloc_error(void *p1, void *p2, char **p3, t_cmd *cmd)
{
	ft_putendl_fd(MALLOC_ERROR_MSG, STDERR_FILENO);
	free_p(p1, p2, NULL, p3);
	free_cmd(cmd);
	free_global_vars();
	exit(EXIT_FAILURE);
}

void	exit_with_code(t_cmd *cmd, int exit_value)
{
	free_cmd(cmd);
	free_tree(&g_shell.tree_root);
	free_global_vars();
	exit(exit_value);
}
