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

t_lstenv *create_nodes(char *data);
void print_list(t_lstenv *head);
void free_list(t_lstenv *head);
void add_env_end(t_lstenv **lstenv, char *data);
void remove_env(t_lstenv **lstenv, char *data);
char *find_env(t_shell *shell, char *data);
char	*get_env_value(t_lstenv *lstenv, char *data);
t_lstenv *get_env_node(char *data);
void	take_env(const char **envp);
void refresh_exported_env();
void add_or_replace_env(char *key, char *value);

#endif