/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:59:15 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/23 16:58:28 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

// todo return correct values..

int go_to_path(char *path)
{
	char *old_pwd;
	char cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	old_pwd = ft_strdup(cwd);
	if (access(path, X_OK) == 0)
	{
		if (chdir(path) == -1)
		{
			perror("minishell: cd");
			exit(EXIT_FAILURE);
		}
		add_or_replace_env("OLDPWD", old_pwd);
		getcwd(cwd, sizeof(cwd));
		add_or_replace_env("PWD", ft_strdup(cwd));
		return 0;
	}
	else
	{
		perror("minishell: cd");
		return 1;
	}
}

int go_to_home()
{
	char *home_path;

	home_path = get_env_value("HOME");
	if (home_path == NULL)
	{
		write(STDERR_FILENO, "cd: HOME not set\n", 17);
		return 1;
	}
	return go_to_path(home_path);
}

// todo return correct values..

int return_to_oldpwd(t_cmd *cmd)
{
	int ret_value;
	char *old_pwd;
	char*tmp;

	old_pwd = get_env_value("OLDPWD");
	if (old_pwd == NULL)
	{
		write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
		return 1;
	}
	tmp = ft_strdup(old_pwd);
	ret_value = go_to_path(tmp);
	if (ret_value == 0)
		ft_pwd(*cmd);
	free(tmp);
	return ret_value;
}

// TODO: make cd print exact error message + return correct value..

int change_directory(t_cmd cmd)
{
	int ret_value;

	if (cmd.argc > 2)
	{
		write(STDERR_FILENO, "cd: too many args\n", 19);
		ret_value = 1;
	}
	else if (cmd.argc == 1)
		ret_value = go_to_home(shell);
	else
	{
		if (ft_strcmp(cmd.argv[1], "-") == 0)
			ret_value = return_to_oldpwd(&cmd);
		else
			ret_value = go_to_path(cmd.argv[1]);
	}
	return ret_value;
}

// int main(int ac, char **av, const char **envp)
// {
// 	t_shell shell;

// 	init_shell(&shell);
// 	take_env(&shell.env_list, envp);
// 	printf("before\n");
// 	printf("%s\n", get_env_lst(shell.env_list, "PWD"));
// 	if (ac == 2)
// 		change_directory(av[1], &shell);
// 	else if (ac == 1)
// 		change_directory(NULL, &shell);
// 	else
// 	{
// 		write(2, "cd: too many arguments\n", 24);
// 		return (1);
// 	}
// 	printf("after\n");
// 	printf("%s\n", get_env_lst(shell.env_list, "PWD"));
// 	printf("%s\n", get_env_lst(shell.env_list, "OLDPWD"));
// 	return (0);
// }