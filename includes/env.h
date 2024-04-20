/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:05:58 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/20 18:45:15 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
#define ENV_H
#include "dynamic_arrays.h"

typedef struct s_lstenv
{
	char *data;
	struct s_lstenv *next;
}   t_lstenv;

typedef struct s_shell
{
	t_lstenv *env_list;
	char * working_dir;
}   t_shell;

void init_shell(t_shell *shell);
t_lstenv *create_nodes(char *data);
void print_list(t_lstenv *head);
void free_list(t_lstenv *head);
void add_env_end(t_lstenv **lstenv, char *data);
void remove_env(t_lstenv **lstenv, char *data);
char *find_env(t_shell *shell, char *data);
void	add_to_env(t_lstenv **lstenv, char *key, char *value);
char	*get_env_value(t_lstenv *lstenv, char *data);
char	*get_env(t_lstenv *lstenv, char *data);
void	take_env(t_lstenv **lstenv, const char **envp);


#endif