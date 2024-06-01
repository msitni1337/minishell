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

void count_matching(char *argv, char **cwdfiles, size_t *res)
{
    bool has_match;
    int i;

    has_match = FALSE;
    i = 0;
    while (cwdfiles[i])
    {
        if (is_pattern_matching(argv, cwdfiles[i]))
        {
            (*res)++;
            has_match = TRUE;
        }
        i++;
    }
    if (has_match == FALSE)
        (*res)++;
}

size_t get_expanded_args_count(char **argv, char **cwdfiles)
{
    size_t count;
    size_t i;

    i = 0;
    count = 0;
    while (argv && argv[i])
    {
        if (ft_strchr(argv[i], '*'))
            count_matching(argv[i], cwdfiles, &count);
        else
            count++;
        i++;
    }
    return count;
}

char **copy_same_argument(char **res, size_t *index, char *argv, char **cwdfiles)
{
    res[*index] = ft_strdup(argv);
    if (res[*index] == NULL)
    {
        free_arr(cwdfiles);
        free_arr(res);
        return NULL;
    }
    (*index)++;
    return res;
}

char **add_matching_filenames(char **res, size_t *index, char *argv, char **cwdfiles)
{
    bool has_match;
    size_t i;

    i = 0;
    has_match = FALSE;
    while (cwdfiles[i])
    {
        if (is_pattern_matching(argv, cwdfiles[i]))
        {
            res[*index] = ft_strdup(cwdfiles[i]);
            if (res[*index] == NULL)
            {
                free_arr(cwdfiles);
                free_arr(res);
                return NULL;
            }
            has_match = TRUE;
            (*index)++;
        }
        i++;
    }
    if (has_match == FALSE)
        return copy_same_argument(res, index, argv, cwdfiles);
    return res;
}

char **init_asterices_expansion_arr(char ***cwdfiles_p, char **argv, size_t *count)
{
    char **res;

    *cwdfiles_p = get_all_cwd_filenames();
    if (*cwdfiles_p == NULL)
        return free_arr(argv);
    sort_arr(*cwdfiles_p);
    *count = get_expanded_args_count(argv, *cwdfiles_p);
    res = malloc(sizeof(char *) * (*count + 1));
    if (res == NULL)
    {
        free_arr(*cwdfiles_p);
        return free_arr(argv);
    }
    res[*count] = NULL;
    return res;
}

char **asterice_expansion_loop(char **res, char **argv, size_t argc, char **cwdfiles)
{
    size_t i;
    size_t j;

    i = 0;
    j = 0;
    while (i < argc)
    {
        if (ft_strchr(argv[i], '*'))
        {
            if (add_matching_filenames(res, &j, argv[i], cwdfiles) == NULL)
                return free_arr(argv);
        }
        else
        {
            if (copy_same_argument(res, &j, argv[i], cwdfiles) == NULL)
                return free_arr(argv);
        }
        i++;
    }
    return res;
}

char **expand_asterices(char **argv, size_t *argc)
{
    char **cwdfiles;
    char **res;
    size_t count;

    res = init_asterices_expansion_arr(&cwdfiles, argv, &count);
    if (res == NULL)
        return NULL;
    if (asterice_expansion_loop(res, argv, *argc, cwdfiles) == NULL)
        return NULL;
    free_arr(argv);
    free_arr(cwdfiles);
    *argc = count;
    return res;
}