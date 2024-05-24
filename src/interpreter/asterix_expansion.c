#include "interpreter.h"

t_list *get_all_cwd_filenames()
{
    char *cwdname;
    DIR *cwdir;
    struct dirent *dir;
    t_list *head;
    t_list *new;

    head = NULL;
    cwdname = getcwd(NULL, 0);
    cwdir = opendir(cwdname);
    free(cwdname);
    if (cwdir == NULL)
        return NULL;
    dir = readdir(cwdir);
    while (dir)
    {
        new = ft_lstnew(ft_strdup(dir->d_name));
        if (new == NULL)
            return NULL;
        ft_lstadd_back(&head, new);
        dir = readdir(cwdir);
    }
    closedir(cwdir);
    return head;
}

int is_pattern_matching(char *pattern, char *fname)
{
    size_t i;
    size_t j;
    bool match_any;

    match_any = FALSE;
    i = 0;
    j = 0;
    while (pattern[i] && fname[j])
    {
        while (pattern[i] = '*')
        {
            match_any = TRUE;
            i++;
        }
        if (match_any == TRUE)
        {
            while (pattern[i] && pattern[i] != '*' && pattern[i] == fname[j])
            {
                i++;
                j++;
            }
            match_any = FALSE;
            continue;
        }
        if (pattern[i] != fname[j])
            return FALSE;
        i++;
        j++;
    }
    return pattern[i] == 0 && fname[i] == 0;
}

char **expand_asterices_argv(char **argv, size_t *argc)
{
    size_t i;
    size_t j;
    t_list *cwdfiles;
    char **res;

    cwdfiles = get_all_cwd_filenames();
    i = 0;
    j = 0;
    while (i < *argc)
    {
        if (ft_strchr(argv[i], '*'))
        {
            t_list *file;
            bool has_match;

            file = cwdfiles;
            has_match = FALSE;
            while (file)
            {
                if (is_pattern_matching(argv[i], file->content))
                {
                    printf("match: %s\n", file->content);
                    res[j] = ft_strdup(file->content);
                    has_match = TRUE;
                    j++;
                }
                file = file->next;
            }
            if (has_match == FALSE)
            {
                printf("not match: %s\n", argv[i]);
                res[j] = argv[i];
                j++;
            }
        }
        else
        {
            res[j] = argv[i];
            j++;
        }
        i++;
    }
    free(argv);
    freelist(cwdfiles);
    return res;
}