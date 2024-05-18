/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:56:12 by nmellal           #+#    #+#             */
/*   Updated: 2024/05/18 01:14:53 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_lstenv *add_env_end(t_lstenv **head, char *key, char *value)
{
	t_lstenv *current;

	if (!*head)
	{
		*head = create_nodes(key, value);
		return *head;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = create_nodes(key, value);
	return current->next;
}
void remove_env(t_lstenv **lstenv, char *key)
{
	t_lstenv *current;
	t_lstenv *prev;

	current = *lstenv;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*lstenv = current->next;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

char *construct_env(char *key, char *value)
{
	char *tmp;
	size_t i;

	i = 0;
	tmp = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	while (key && *key)
	{
		tmp[i] = *key;
		key++;
		i++;
	}
	tmp[i] = '=';
	i++;
	while (value && *value)
	{
		tmp[i] = *value;
		value++;
		i++;
	}
	return tmp;
}

size_t exp_env_size()
{
	t_lstenv *lst;
	size_t size;

	size = 0;
	lst = shell.env_list;
	while (lst)
	{
		if (lst->is_set == TRUE)
			size++;
		lst = lst->next;
	}
	return (size);
}

char **get_env_arr()
{
	char **res;
	size_t i;
	t_lstenv *current;

	res = malloc(sizeof(char**) * (exp_env_size() + 1));
	current = shell.env_list;
	i = 0;
	while (current)
	{
		if (current->is_set == TRUE)
		{
			res[i] = construct_env(current->key, current->value);
			i++;
		}
		current = current->next;
	}
	res[i] = NULL;
	return res;
}

t_lstenv *create_nodes(char *key, char *value)
{
	t_lstenv *new_node;

	new_node = malloc(sizeof(t_lstenv));
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->is_set = TRUE;
	new_node->next = NULL;
	return (new_node);
}

t_lstenv *add_or_replace_env(char *key, char *value)
{
	t_lstenv *node;

	node = get_env_node(key);
	if (node)
	{
		if (value)
		{
			free(node->value);
			node->value = ft_strdup(value);
			node->is_set = TRUE;
		}
	}
	else
	{
		if (value)
			node = add_env_end(&(shell.env_list), ft_strdup(key), ft_strdup(value));
		else
		{
			node = add_env_end(&(shell.env_list), ft_strdup(key), NULL);		
			node->is_set = FALSE;
		}
	}
	return node;
}

void increment_shlvl()
{
	t_lstenv *node;
	int shlvl;

	node = get_env_node("SHLVL");
	if (node)
	{
		shlvl = ft_atoi(node->value);
		shlvl++;
		add_or_replace_env("SHLVL", ft_itoa(shlvl));
	}
	else
	{
		add_or_replace_env("SHLVL", ft_itoa(1));
	}
}

void take_env(const char **envp)
{
	t_lstenv *result;
	int i;

	result = NULL;
	i = 0;
	while (envp && envp[i])
	{
		char *key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		char *value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		add_env_end(&result, key, value);
		i++;
	}
	shell.env_list = result;
	increment_shlvl();
}

/*
void refresh_exported_env()
{
	t_lstenv *env_lst;
	char **result;
	size_t count;
	size_t i;

	// todo free old exported env
	result = NULL;
	env_lst = shell.env_list;
	sort_env_list(&(shell.env_list));
	count = lstenv_size(env_lst);
	if (count > 0)
	{
		result = malloc(sizeof(char **) * (count + 1));
		i = 0;
		while (env_lst)
		{
			result[i] = env_lst->data;
			i++;
			env_lst = env_lst->next;
		}
		result[i] = NULL;
	}
	shell.exported_env = result;
}
*/

t_lstenv *get_env_node(char *key)
{
	t_lstenv *current;

	current = shell.env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return current;
		current = current->next;
	}
	return (NULL);
}

char *get_env_value(char *key)
{
	t_lstenv *current;

	if (ft_strcmp(key, "?") == 0)
		return ft_itoa(shell.last_exit_value);
	if (ft_strcmp(key, "$") == 0)
		return ft_itoa(getpid());
	current = shell.env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return current->value;
		current = current->next;
	}
	return (NULL);
}

// int	main(int ac, char **av, const char **envp)
// {
// 	t_shell env;

// 	USED(ac);
// 	USED(av);
// 	init_shell(&env);
// 	take_env(&env.env_list, envp);
// 	printf("%s\n", find_env_key(env.env_list, "HOME"));
// }
