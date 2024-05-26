#include "interpreter.h"

size_t parse_key_count(const char *s)
{
    size_t count;

    count = 0;
    if (ft_isdigit(s[count]))
        return ++count;
    else if (eq(s[count], '$') || eq(s[count], '?') || eq(s[count], '*'))
        return ++count;
    while (s[count])
    {
        if (!ft_isalnum(s[count]) && !eq(s[count], '_'))
            break;
        count++;
    }
    return count;
}

void copy_var_value(char *res, t_string *string, size_t *i)
{
    int count;
    char *tmp;
    char buff[BUFSIZ];

    (string->count)--;
    (string->s)++;
    count = parse_key_count(string->s);
    if (count && count < BUFSIZ - 1)
    {
        ft_strlcpy(buff, string->s, count + 1);
        tmp = get_env_value(buff);
        if (tmp)
            *i += ft_strlcpy(res + *i, tmp, INT32_MAX);
        (string->count) -= count;
        (string->s) += count;
    }
    else
    {
        res[*i] = '$';
        (*i)++;
    }
}