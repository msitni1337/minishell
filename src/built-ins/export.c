/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmellal <nmellal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:06:01 by nmellal           #+#    #+#             */
/*   Updated: 2024/04/23 16:06:13 by nmellal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "built-ins.h"

size_t get_sorted_env_count()
{
    t_lstenv *env_lst;
    size_t count;
    
    count = 0;
    env_lst = shell.env_list;
    while (env_lst)
    {
        count++;
        env_lst = env_lst->next;
    }
    return count;
}

t_lstenv**get_sorted_env()
{
    t_lstenv *env_lst;
    t_lstenv**res;
    size_t count;
    size_t i;

    count = get_sorted_env_count();
    res = malloc(sizeof(t_lstenv) * (count + 1));
    if (res)
    {
        i = 0;
        while (i < count)
        {
            res[i] = env_lst;
            env_lst = env_lst->next;
            i++;
        }
        res[i] = NULL;
    }
    return res;    
}

int print_exported_env(t_cmd cmd)
{
    char *tmp;
    char**sorted_env;
    size_t i;

    sorted_env = get_sorted_env();
    i = 0;
    while ()
    {
        if (ft_strncmp(tmp, "_=", 2))
        {
            ft_putstr_fd("declare -x ", cmd.outfile);
            write(cmd.outfile, tmp, ft_strchr(tmp, '=') + 1 - tmp);
            ft_putchar_fd('"', cmd.outfile);
            ft_putstr_fd(ft_strchr(tmp, '=') + 1, cmd.outfile);
            ft_putchar_fd('"', cmd.outfile);
            write(cmd.outfile, "\n", 1);
        }
        i++;
    }
    return 0;
}

int ft_export(t_cmd cmd)
{
    if (cmd.argc == 1)
    {
        return print_exported_env(cmd);
    }
    else
    {
        ;
    }

    return 0;
}