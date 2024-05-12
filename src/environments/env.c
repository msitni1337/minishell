/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:56:12 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/23 17:08:23 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void add_env_end(t_lstenv **head, char *data)
{
	t_lstenv *current;

	if (!*head)
	{
		*head = create_nodes(data);
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = create_nodes(data);
}
char *find_env(t_shell *shell, char *data)
{
	t_lstenv *current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strncmp(current->data, data, ft_strlen(data)) == 0)
			return (current->data);
		current = current->next;
	}
	return (NULL);
}
void remove_env(t_lstenv **lstenv, char *data)
{
	t_lstenv *current;
	t_lstenv *prev;

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
			return;
		}
		prev = current;
		current = current->next;
	}
}

void free_list(t_lstenv *head)
{
	t_lstenv *current;
	t_lstenv *next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

t_lstenv *create_nodes(char *data)
{
	t_lstenv *new_node;

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

void ft_swap(char **p1, char **p2)
{
	char *tmp;

	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void sort_env_list(t_lstenv **head)
{
	bool is_swapped;
	t_lstenv *current;
	t_lstenv *prev;

	if (head == NULL || *head == NULL || (*head)->next == NULL)
		return;
	is_swapped = TRUE;
	while (is_swapped == TRUE)
	{
		is_swapped = FALSE;
		prev = *head;
		current = prev->next;
		while (current)
		{
			if (ft_strcmp(current->data, prev->data) < 0)
			{
				ft_swap(&(current->data), &(prev->data));
				is_swapped = TRUE;
			}
			prev = current;
			current = prev->next;
		}
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

void add_or_replace_env(char *key, char *value)
{
	t_lstenv *node;
	char *tmp;

	tmp = construct_env(key, value);
	key = ft_strjoin(key, "=");
	node = get_env_node(key);
	if (node)
	{
		free(node->data);
		node->data = tmp;
	}
	else
	{
		add_env_end(&(shell.env_list), tmp);
	}
	refresh_exported_env();

	// -> free those when galloc is available
	// free(key);
	// free(value);
}

void increment_shlvl()
{
	t_lstenv *node;
	char *key_end;
	int shlvl;

	node = get_env_node("SHLVL=");
	if (node)
	{
		key_end = ft_strchr(node->data, '=');
		shlvl = ft_atoi(key_end + 1);
		shlvl++;
		add_or_replace_env("SHLVL", ft_itoa(shlvl));
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
		add_env_end(&result, ft_strdup(envp[i]));
		i++;
	}
	shell.env_list = result;
	increment_shlvl();
	refresh_exported_env();
}

size_t lstenv_size(t_lstenv *lst)
{
	size_t size;

	size = 0;
	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}

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

t_lstenv *get_env_node(char *key)
{
	t_lstenv *current;

	current = shell.env_list;
	while (current)
	{
		if (ft_strncmp(current->data, key, ft_strlen(key)) == 0)
			return current;
		current = current->next;
	}
	return (NULL);
}

char *get_env_value(t_lstenv *lstenv, char *data)
{
	t_lstenv *current;
	char *value;

	if (ft_strcmp(data, "?") == 0)
		return ft_itoa(shell.last_exit_value);
	if (ft_strcmp(data, "$") == 0)
		return ft_itoa(getpid());
	current = lstenv;
	while (current)
	{
		if (ft_strncmp(current->data, data, ft_strlen(data)) == 0)
		{
			if (current->data[ft_strlen(data)] == '=')
			{
				value = ft_strchr(current->data, '=') + 1;
				return (value);
			}
		}
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
