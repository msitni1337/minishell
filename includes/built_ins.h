/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:43:09 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 09:54:58 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H
# include "environment.h"
# include "shared.h"
# include <errno.h>
# include <fcntl.h>

/* echo */
int			ft_echo(t_cmd cmd);

/* cd */
int			change_directory(t_cmd cmd);

/* pwd */
int			ft_pwd(t_cmd cmd);

/* export */
int			ft_export(t_cmd cmd);
t_lstenv	**get_sorted_env(void);
int			print_exported_env(t_cmd cmd);
void		ft_swap_env_lst(t_lstenv **p1, t_lstenv **p2);
char		*get_key_and_mode(char *arg, int *append);

/* unset */
int			ft_unset(t_cmd cmd);

/* env */
int			ft_env(t_cmd cmd);

/* exit */
int			ft_exit(t_cmd cmd);

/* common */
int			check_key_is_valid(char *key);
int			key_not_valid(char *cmd, char *key);

/* freeing */
void		*free_p(void *p1, void *p2, void *p3, char **p4);

#endif