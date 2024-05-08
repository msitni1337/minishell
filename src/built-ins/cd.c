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

int go_to_home()
{
	char *old_path;
	char *new_path;
	char buff[PATH_MAX];

	if (getcwd(buff, sizeof(buff)) != NULL)
		old_path = ft_strdup(buff);
	new_path = get_env_value(shell.env_list, "HOME");
	if (new_path == NULL)
	{
		write(STDERR_FILENO, "cd: HOME not set\n", 17);
		return 1;
	}
	if (chdir(new_path) == -1)
	{
		perror("minishell: cd");
		exit(EXIT_FAILURE);
	}
	replace_env("OLDPWD", old_path);
	replace_env("PWD", ft_strdup(new_path));
	return 0;
}

// todo return correct values..

int return_to_oldpwd(t_cmd *cmd)
{
	char *old_path;
	char *new_path;
	char buff[PATH_MAX];

	if (getcwd(buff, sizeof(buff)) != NULL)
		old_path = ft_strdup(buff);
	new_path = get_env_value(shell.env_list, "OLDPWD=");
	if (new_path == NULL)
	{
		write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
		return 1;
	}
	if (chdir(new_path) == -1)
	{
		perror("minishell: cd");
		exit(EXIT_FAILURE);
	}

	ft_putendl_fd(new_path, cmd->outfile);

	replace_env("OLDPWD", old_path);
	replace_env("PWD", ft_strdup(new_path));
	return 0;
}

// todo return correct values..

int go_to_path(char *path)
{
	int ret_value;
	char *old_path;
	char buff[PATH_MAX];

	ret_value = access(path, X_OK);
	if (ret_value == -1)
	{
		perror("minishell: cd");
		exit(EXIT_FAILURE); // keep exiting now untill gexit is implemented
	}
	else if (ret_value == 0)
	{
		if (getcwd(buff, sizeof(buff)) != NULL)
		{
			old_path = ft_strdup(buff);
			if (chdir(path) == -1)
			{
				perror("minishell: cd");
				exit(EXIT_FAILURE); // keep exiting now untill gexit is implemented
			}
			getcwd(buff, sizeof(buff));
			replace_env("OLDPWD", old_path);
			replace_env("PWD", ft_strdup(buff));
		}
	}
	else
	{
		perror("minishell: cd");
	}
	return ret_value;
}

// TODO: make cd print exact error message + return correct value..

int change_directory(t_cmd cmd)
{
	int ret_value;

	if (cmd.argc > 2)
	{
		// todo print error "bash too many args"
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