/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:59:15 by nmellal           #+#    #+#             */
/*   Updated: 2024/05/18 19:31:42 by msitni           ###   ########.fr       */
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

	ret_value = 0;
	if (cmd.argc > 2)
	{
		write(STDERR_FILENO, "cd: too many args\n", 19);
		ret_value = 1;
	}
	else if (cmd.argc == 1)
		ret_value = go_to_home();
	else
	{
		if (ft_strcmp(cmd.argv[1], "-") == 0)
			ret_value = return_to_oldpwd(&cmd);
		else if (ft_strlen(cmd.argv[1]))
			ret_value = go_to_path(cmd.argv[1]);
	}
	return ret_value;
}