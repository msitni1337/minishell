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

/* START OF NORMED FUNCTIONS */

size_t get_exported_env_count()
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

void ft_swap_env_lst(t_lstenv **p1, t_lstenv **p2)
{
    t_lstenv *tmp;

    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void sort_env_list(t_lstenv **env_list)
{
    size_t i;
    bool is_swapped;

    is_swapped = TRUE;
    while (is_swapped == TRUE)
    {
        is_swapped = FALSE;
        i = 1;
        while (env_list[i - 1] && env_list[i])
        {
            if (ft_strcmp(env_list[i - 1]->key, env_list[i]->key) > 0)
            {
                ft_swap_env_lst(&env_list[i - 1], &env_list[i]);
                is_swapped = TRUE;
            }
            i++;
        }
    }
}

t_lstenv **get_sorted_env()
{
    t_lstenv *env_lst;
    t_lstenv **res;
    size_t count;
    size_t i;

    count = get_exported_env_count();
    env_lst = shell.env_list;
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
        sort_env_list(res);
    }
    return res;
}

void print_env(t_cmd *cmd, t_lstenv *env)
{
    ft_putstr_fd("declare -x ", cmd->outfile);
    ft_putstr_fd(env->key, cmd->outfile);
    if (env->is_set)
    {
        ft_putstr_fd("=\"", cmd->outfile);
        ft_putstr_fd(env->value, cmd->outfile);
        ft_putchar_fd('"', cmd->outfile);
    }
    write(cmd->outfile, "\n", 1);
}

int print_exported_env(t_cmd cmd)
{
    t_lstenv **sorted_env;
    size_t i;

    sorted_env = get_sorted_env();
    if (sorted_env == NULL)
        malloc_error(&cmd);
    i = 0;
    while (sorted_env && sorted_env[i])
    {
        if (ft_strcmp(sorted_env[i]->key, "_"))
            print_env(&cmd, sorted_env[i]);
        i++;
    }
    free(sorted_env);
    return 0;
}

char *get_key_and_mode(char *arg, bool *append)
{
    char *tmp;

    *append = FALSE;
    tmp = ft_strchr(arg, '=');
    if (tmp > arg && *(tmp - 1) == '+')
    {
        *append = TRUE;
        tmp--;
    }
    return ft_substr(arg, 0, tmp - arg);
}

/* END OF NORMED FUNCTIONS */

void add_env(char *key, char *arg, bool append)
{
    char *value;
    char *tmp;
    t_lstenv *node;

    value = ft_strdup(ft_strchr(arg, '=') + 1);
    if (append == TRUE)
    {
        node = add_or_replace_env(key, NULL);
        node->is_set = TRUE;
        tmp = ft_strjoin(node->value, value);
        free(node->value);
        node->value = tmp;
    }
    else
    {
        add_or_replace_env(key, value);
    }
    free(value);
}

int export_env(char *arg)
{
    int ret_value;
    bool append;
    char *key;

    key = get_key_and_mode(arg, &append);
    ret_value = 0;
    if (ft_strlen(key) && check_key_is_valid(key) == 0)
    {
        if (ft_strchr(arg, '='))
            add_env(key, arg, append);
        else
            add_or_replace_env(key, NULL);
    }
    else
        ret_value = key_not_valid("export", key);
    free(key);
    return ret_value;
}

int parse_export(t_cmd cmd)
{
    int tmp;
    int ret_value;
    size_t i;

    i = 1;
    ret_value = 0;
    while (i < cmd.argc)
    {
        tmp = export_env(cmd.argv[i]);
        if (ret_value == 0)
            ret_value = tmp;
        i++;
    }
    return ret_value;
}

int ft_export(t_cmd cmd)
{
    if (cmd.argc == 1)
        return print_exported_env(cmd);
    return parse_export(cmd);
}