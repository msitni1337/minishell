/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:08:32 by nmellal           #+#    #+#             */
/*   Updated: 2024/05/18 19:42:26 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <fcntl.h>
#include <errno.h>
#include "env.h"
#include "shared.h"
#include "cmd.h"

#define PATH_MAX 4096

/* echo */
int ft_echo(t_cmd cmd);

/* cd */
int change_directory(t_cmd cmd);
int	go_to_path(char *path);
int return_to_oldpwd();
int go_to_home();

/* pwd */
int	ft_pwd(t_cmd cmd);

/* export */
int ft_export(t_cmd cmd);

/* unset */
int	ft_unset(t_cmd cmd);

/* env */
int	ft_env(t_cmd cmd);

/* exit */
int ft_exit(t_cmd cmd);

/* common */
int check_key_is_valid(char*key);
int key_not_valid(char *cmd, char*key);

#endif