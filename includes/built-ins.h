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
int     ft_echo(int ac, char **av);
void	check_echo_options(char **av);
/* cd */

int change_directory(t_cmd cmd);
int	go_to_path(char *path);
int return_to_oldpwd();
int go_to_home();

/* pwd */
int	ft_pwd(int ac, char **av);
#endif