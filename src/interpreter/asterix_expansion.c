#include "interpreter.h"

void ft_swap_lst(char **p1, char **p2)
{
    char *tmp;

    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void sort_arr(char **list)
{
    size_t i;
    bool is_swapped;

    is_swapped = TRUE;
    while (is_swapped == TRUE)
    {
        is_swapped = FALSE;
        i = 1;
        while (list[i - 1] && list[i])
        {
            if (ft_strcmp(list[i - 1], list[i]) > 0)
            {
                ft_swap_lst(&list[i - 1], &list[i]);
                is_swapped = TRUE;
            }
            i++;
        }
    }
}

char **get_all_cwd_filenames()
{
    char *tmp;
    DIR *cwdir;
    struct dirent *dir;
    t_darr res;

    res = init_da(sizeof(char *));
    tmp = getcwd(NULL, 0);
    if (tmp == NULL)
        return NULL;
    cwdir = opendir(tmp);
    free(tmp);
    if (cwdir == NULL)
        return NULL;
    dir = readdir(cwdir);
    while (dir)
    {
        tmp = ft_strdup(dir->d_name);
        if (tmp == NULL)
            return free_p(NULL, NULL, NULL, res.data);
        if (add_to_arr(&res, &tmp) == NULL)
            return free_p(tmp, NULL, NULL, res.data);
        dir = readdir(cwdir);
    }
    closedir(cwdir);
    sort_arr(res.data);
    return res.data;
}

int matching_loop(char **pattern, char *name, char *star, char *star_pos)
{
    while (*name)
    {
        if (**pattern == *name)
        {
            name++;
            (*pattern)++;
            continue;
        }
        if (**pattern == '*')
        {
            star = (*pattern)++;
            star_pos = name;
            continue;
        }
        if (star)
        {
            (*pattern) = star + 1;
            name = ++star_pos;
            continue;
        }
        return FALSE;
    }
    return TRUE;
}

int is_pattern_matching(char *pattern, char *name)
{
    if (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0)
        return FALSE;
    if (*pattern != '.' && *name == '.')
        return FALSE;
    if (matching_loop(&pattern, name, NULL, name) == FALSE)
        return FALSE;
    while (*pattern == '*')
        pattern++;
    return *pattern == 0;
}

size_t get_expanded_args_count(char **argv, char **cwdfiles)
{
    size_t res;
    size_t i;
    size_t j;
    bool has_match;

    i = 0;
    res = 0;
    while (argv && argv[i])
    {
        if (ft_strchr(argv[i], '*'))
        {
            has_match = FALSE;
            j = 0;
            while (cwdfiles[j])
            {
                if (is_pattern_matching(argv[i], cwdfiles[j]))
                {
                    res++;
                    has_match = TRUE;
                }
                j++;
            }
            if (has_match == FALSE)
                res++;
        }
        else
            res++;
        i++;
    }
    return res;
}

char **expand_asterices(char **argv, size_t *argc)
{
    size_t i;
    size_t j;
    size_t y;
    char **cwdfiles;
    char **res;
    size_t count;

    cwdfiles = get_all_cwd_filenames();
    if (cwdfiles == NULL)
        return free_arr(argv);
    count = get_expanded_args_count(argv, cwdfiles);
    res = malloc(sizeof(char *) * (count + 1));
    if (res == NULL)
    {
        free_arr(cwdfiles);
        return free_arr(argv);
    }
    res[count] = NULL;
    i = 0;
    j = 0;
    while (i < *argc)
    {
        if (ft_strchr(argv[i], '*'))
        {
            bool has_match;

            y = 0;
            has_match = FALSE;
            while (cwdfiles[y])
            {
                if (is_pattern_matching(argv[i], cwdfiles[y]))
                {
                    // printf("match: %s\n", file->content);
                    res[j] = ft_strdup(cwdfiles[y]);
                    if (res[j] == NULL)
                    {
                        free_arr(cwdfiles);
                        free_arr(res);
                        free_arr(argv);
                        return NULL;
                    }
                    has_match = TRUE;
                    j++;
                }
                y++;
            }
            if (has_match == FALSE)
            {
                // printf("not match: %s\n", argv[i]);
                res[j] = ft_strdup(argv[i]);
                if (res[j] == NULL)
                {
                    free_arr(cwdfiles);
                    free_arr(res);
                    free_arr(argv);
                    return NULL;
                }
                j++;
            }
        }
        else
        {
            res[j] = ft_strdup(argv[i]);
            if (res[j] == NULL)
            {
                free_arr(cwdfiles);
                free_arr(res);
                free_arr(argv);
                return NULL;
            }
            j++;
        }
        i++;
    }
    free_arr(argv);
    free_arr(cwdfiles);
    *argc = count;
    return res;
}