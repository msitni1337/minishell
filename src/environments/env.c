/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:56:12 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/20 18:43:46 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "shared.h"
#include <stdio.h>

void	init_shell(t_shell *shell)
{
	shell->env_list = NULL;
	shell->working_dir = NULL;
}

void	print_list(t_lstenv *head)
{
	t_lstenv	*current;

	current = head;
	while (current)
	{
		printf("%s\n", current->data);
		current = current->next;
	}
}
void	add_env_end(t_lstenv **head, char *data)
{
	t_lstenv	*current;

	if (!*head)
	{
		*head = create_nodes(data);
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = create_nodes(data);
}
char	*find_env(t_shell *shell, char *data)
{
	t_lstenv	*current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strncmp(current->data, data, ft_strlen(data)) == 0)
			return (current->data);
		current = current->next;
	}
	return (NULL);
}
void	remove_env(t_lstenv **lstenv, char *data)
{
	t_lstenv	*current;
	t_lstenv	*prev;

	current = *lstenv;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->data, data, ft_strlen(data)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*lstenv = current->next;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	free_list(t_lstenv *head)
{
	t_lstenv	*current;
	t_lstenv	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

t_lstenv	*create_nodes(char *data)
{
	t_lstenv	*new_node;

	new_node = malloc(sizeof(t_lstenv));
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->data = data;
	new_node->next = NULL;
	return (new_node);
}
void	take_env(t_lstenv **lstenv, const char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		add_env_end(lstenv, ft_strdup(envp[i]));
		i++;
	}
}
char	*get_env(t_lstenv *lstenv, char *data)
{
	t_lstenv	*current;

	current = lstenv;
	while (current)
	{
		if (ft_strncmp(current->data, data, ft_strlen(data)) == 0)
			return (current->data);
		current = current->next;
	}
	return (NULL);
}

char	*get_env_value(t_lstenv *lstenv, char *data)
{
	t_lstenv	*current;
	char		*value;

	current = lstenv;
	while (current)
	{
		if (ft_strncmp(current->data, data, ft_strlen(data)) == 0)
		{
			value = ft_strchr(current->data, '=') + 1;
			return (value);
		}
		current = current->next;
	}
	return (NULL);
}

void	add_to_env(t_lstenv **lstenv, char *key, char *value)
{
	char	*new_env;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	new_env = ft_strjoin(tmp, value);
	if (get_env(*lstenv, key))
		remove_env(lstenv, key);
	add_env_end(lstenv, new_env);
	free(tmp);
}

int	main(int ac, char **av, const char **envp)
{
	t_shell env;

	USED(ac);
	USED(av);
	init_shell(&env);
	take_env(&env.env_list, envp);
	printf("%s\n", get_env(env.env_list, "HOME"));
	add_to_env(&env.env_list, "HOME", "/home/msitini");
	printf("%s\n", get_env(env.env_list, "HOME"));
}