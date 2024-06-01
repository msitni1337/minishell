#include "interpreter.h"

size_t parse_key_count(const char *s)
{
    size_t count;

    count = 0;
    if (s[count] == 0 || ft_isspace(s[count]))
        return -1;
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

void perform_var_value_copy(char* res, t_string *string, size_t *i, int key_count)
{
    char key[BUFF_SZ];
    char *value;

    ft_strlcpy(key, string->s, key_count + 1);
    if (ft_strcmp(key, "$") == 0)
        *i += sitoa(res + *i, getpid());
    else if (ft_strcmp(key, "?") == 0)
        *i += sitoa(res + *i, shell.last_exit_value);
    else
    {
        value = get_env_value(key);
        if (value)
            *i += ft_strlcpy(res + *i, value, INT32_MAX);
    }
    (string->count) -= key_count;
    (string->s) += key_count;
}

void copy_var_value(char *res, t_string *string, size_t *i)
{
    int key_count;

    (string->count)--;
    (string->s)++;
    key_count = parse_key_count(string->s);
    if (key_count > 0 && key_count < BUFF_SZ - 1)
    {
         perform_var_value_copy(res, string, i, key_count);
    }
    else if (key_count == -1)
    {
        res[*i] = '$';
        (*i)++;
    }
}