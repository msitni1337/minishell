/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:08:32 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/23 16:06:28 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <fcntl.h>
#include <errno.h>
#include "env.h"

#define PATH_MAX 4096

/* echo */
void	ft_echo(int ac, char **av);
void	check_echo_options(char **av);
/* cd */

void change_directory(char *path, t_shell *shell);
void	go_to_path(char *path, t_shell *shell);
void return_to_oldpwd(t_shell *shell);
void go_to_home(t_shell *shell);

/* pwd */
void	ft_pwd(int ac, char **av);
#endif