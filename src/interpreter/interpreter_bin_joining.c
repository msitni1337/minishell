/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_bin_joining.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:52:05 by nmellal           #+#    #+#             */
/*   Updated: 2024/06/02 18:52:06 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

char	*check_bin_path(t_string path, char *cmd)
{
	struct stat	fstat;
	char		*tmp;
	char		*fullpath;

	if (path.count == 0)
	{
		fullpath = ft_strdup(cmd);
	}
	else
	{
		fullpath = ft_substr(path.s, 0, path.count);
		tmp = ft_strjoin(fullpath, "/");
		free(fullpath);
		fullpath = ft_strjoin(tmp, cmd);
		free(tmp);
	}
	if (stat(fullpath, &fstat) == 0 && fstat.st_mode & S_IFREG
		&& fstat.st_mode & S_IXUSR)
	{
		return (fullpath);
	}
	free(fullpath);
	return (NULL);
}

char	*get_binary_path(char *bin_name, char *raw_path)
{
	t_string	paths;
	char		*full_path;
	size_t		i;

	paths.s = raw_path;
	paths.count = 0;
	if (raw_path == NULL)
		return (check_bin_path(paths, bin_name));
	i = 0;
	while (raw_path && raw_path[i])
	{
		if (raw_path[i] == ':')
		{
			full_path = check_bin_path(paths, bin_name);
			if (full_path)
				return (full_path);
			paths.s = raw_path + i + 1;
			paths.count = 0;
		}
		else
			paths.count++;
		i++;
	}
	return (check_bin_path(paths, bin_name));
}

int	get_cmd_path(t_cmd *cmd)
{
	if (ft_strchr(cmd->argv[0], '/'))
	{
		if (access(cmd->argv[0], F_OK))
		{
			perror(cmd->argv[0]);
			return (127);
		}
		if (access(cmd->argv[0], X_OK))
		{
			perror(cmd->argv[0]);
			return (126);
		}
		cmd->bin_path = cmd->argv[0];
	}
	else
	{
		cmd->bin_path = get_binary_path(cmd->argv[0], get_env_value("PATH"));
		if (cmd->bin_path == NULL)
		{
			print_error(cmd->argv[0], "command not found");
			return (127);
		}
	}
	return (0);
}
