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
	char *new_path;
	char old_path[PATH_MAX];

	if (getcwd(old_path, sizeof(old_path)) != NULL)
		shell.working_dir = ft_strdup(old_path);
	new_path = get_env_value(shell.env_list, "HOME");
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
	add_to_env(&shell.env_list, "OLDPWD", shell.working_dir);
	add_to_env(&shell.env_list, "PWD", new_path);
}

// todo return correct values..

int return_to_oldpwd(t_cmd*cmd)
{
	char *new_path;
	char old_path[PATH_MAX];

	if (getcwd(old_path, sizeof(old_path)) != NULL)
		shell.working_dir = ft_strdup(old_path);
	new_path = get_env_value(shell.env_list, "OLDPWD");
	if (new_path == NULL)
	{
		write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
		return;
	}
	if (chdir(new_path) == -1)
	{
		perror("minishell: cd");
		exit(EXIT_FAILURE);
	}

	ft_putendl_fd(new_path, cmd->outfile);
	
	add_to_env(&shell.env_list, "OLDPWD", shell.working_dir);
	add_to_env(&shell.env_list, "PWD", new_path);
}


// todo return correct values..

int	go_to_path(char *path)
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
			shell.working_dir = ft_strdup(old_path);
			if (chdir(path) == -1)
			{
				perror("minishell: cd");
				exit(EXIT_FAILURE);
			}
		}
		add_to_env(&shell.env_list, "OLDPWD", shell.working_dir);
		add_to_env(&shell.env_list, "PWD", getcwd(NULL, 0));
	}
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