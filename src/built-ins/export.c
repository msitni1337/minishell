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

// TODO: Implement export built-in

int print_exported_env(t_cmd cmd)
{
    char *tmp;
    size_t i;
    assert(!"NOT IMPLEMENTD");
    i = 0;
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