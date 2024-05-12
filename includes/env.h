/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:05:58 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/23 17:10:55 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
#define ENV_H
#include "shell.h"

t_lstenv *create_nodes(char *key, char *value);
void	take_env(const char **envp);
t_lstenv* add_env_end(t_lstenv **head, char *key, char *value);
void remove_env(t_lstenv **lstenv, char *key);
char	*get_env_value(char *key);
t_lstenv *get_env_node(char *data);
t_lstenv* add_or_replace_env(char *key, char *value);
char **get_env_arr();

#endif