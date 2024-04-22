/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:59:15 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/22 15:51:05 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

static int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void go_to_home(t_shell *shell)
{
	char *new_path;
	char old_path[PATH_MAX];

	if (getcwd(old_path, sizeof(old_path)) != NULL)
		shell->working_dir = ft_strdup(old_path);
	new_path = get_env_value(shell->env_list, "HOME");
	if (new_path == NULL)
	{
		write(2, "cd: HOME not set\n", 17);
		return;
	}
	if (chdir(new_path) == -1)
	{
		perror("minishell: cd");
		exit(EXIT_FAILURE);
	}
	add_to_env(&shell->env_list, "OLDPWD", shell->working_dir);
	add_to_env(&shell->env_list, "PWD", new_path);
}
void return_to_oldpwd(t_shell *shell)
{
	char *new_path;
	char old_path[PATH_MAX];

	if (getcwd(old_path, sizeof(old_path)) != NULL)
		shell->working_dir = ft_strdup(old_path);
	new_path = get_env_value(shell->env_list, "OLDPWD");
	if (new_path == NULL)
	{
		write(2, "cd: OLDPWD not set\n", 19);
		return;
	}
	if (chdir(new_path) == -1)
	{
		perror("minishell: cd");
		exit(EXIT_FAILURE);
	}
	add_to_env(&shell->env_list, "OLDPWD", shell->working_dir);
	add_to_env(&shell->env_list, "PWD", new_path);
}

void	go_to_path(char *path, t_shell *shell)
{
	char old_path[PATH_MAX];

	if (access(path, F_OK) == -1)
	{
		perror("minishell: cd");
		exit(EXIT_FAILURE);
	}
	else
	{
		if (getcwd(old_path, sizeof(old_path)) != NULL)
		{
			shell->working_dir = ft_strdup(old_path);
			if (chdir(path) == -1)
			{
				perror("minishell: cd");
				exit(EXIT_FAILURE);
			}
		}
		add_to_env(&shell->env_list, "OLDPWD", shell->working_dir);
		add_to_env(&shell->env_list, "PWD", getcwd(NULL, 0));
	}
}

void change_directory(char *path, t_shell *shell)
{
	if (path == NULL)
		go_to_home(shell);
	else
	{
		if (ft_strcmp(path, "-") == 0)
			return_to_oldpwd(shell);
		else
			go_to_path(path, shell);
	}
}

int main(int ac, char **av, const char **envp)
{
	t_shell shell;

	init_shell(&shell);
	take_env(&shell.env_list, envp);
	printf("before\n");
	printf("%s\n", get_env_lst(shell.env_list, "PWD"));
	if (ac == 2)
		change_directory(av[1], &shell);
	else if (ac == 1)
		change_directory(NULL, &shell);
	else
	{
		write(2, "cd: too many arguments\n", 24);
		return (1);
	}
	printf("after\n");
	printf("%s\n", get_env_lst(shell.env_list, "PWD"));
	printf("%s\n", get_env_lst(shell.env_list, "OLDPWD"));
	return (0);
}