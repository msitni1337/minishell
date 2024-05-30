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

int count_num_chars(long n)
{
    int i;

    i = 1;
    if (n < 0)
    {
        n = -n;
        i++;
    }
    while (n > 9)
    {
        n /= 10;
        i++;
    }
    return (i);
}

void copy_num_to_str(char *str, size_t *i, long nbr)
{
    long tmp;

    if (nbr < 0)
    {
        nbr = -nbr;
        str[*i] = '-';
        (*i)++;
    }
    if (nbr >= 0 && nbr <= 9)
    {
        tmp = nbr;
        while (tmp > 9)
            tmp /= 10;
        str[*i] = '0' + tmp;
        (*i)++;
        return;
    }
    copy_num_to_str(str, i, nbr / 10);
    copy_num_to_str(str, i, nbr % 10);
}

size_t sitoa(char *buff, long nbr)
{
    size_t i;

    if (buff == NULL)
        return 0;
    i = 0;
    copy_num_to_str(buff, &i, nbr);
    return i;
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
        if (ft_strcmp(buff, "$") == 0)
            *i += sitoa(res + *i, getpid());
        else if (ft_strcmp(buff, "?") == 0)
            *i += sitoa(res + *i, shell.last_exit_value);
        else
        {
            tmp = get_env_value(buff);
            if (tmp)
                *i += ft_strlcpy(res + *i, tmp, INT32_MAX);
        }
        (string->count) -= count;
        (string->s) += count;
    }
    else
    {
        res[*i] = '$';
        (*i)++;
    }
}