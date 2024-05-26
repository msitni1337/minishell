#include "interpreter.h"

void count_var_len(t_string string, size_t *i, size_t *len)
{
    int count;
    char buff[BUFSIZ];

    (*i)++;
    count = parse_key_count(string.s + *i);
    if (count && count < BUFSIZ - 1)
    {
        ft_strlcpy(buff, string.s + *i, count + 1);
        *len += ft_strlen(get_env_value(buff));
        *i += count;
    }
}

t_expansion_state count_normal_mode(t_string string, size_t *i, size_t *len, int expand_vars)
{
    if (string.s[*i] == '\'')
    {
        (*i)++;
        return SQUOTE_STATE;
    }
    else if (string.s[*i] == '"')
    {
        (*i)++;
        return DQUOTE_STATE;
    }
    else if (expand_vars && string.s[*i] == '$')
    {
        count_var_len(string, i, len);
    }
    else
    {
        (*i)++;
        (*len)++;
    }
    return NORMAL;
}

t_expansion_state count_dquote_mode(t_string string, size_t *i, size_t *len, int expand_vars)
{
    if (string.s[*i] == '"')
    {
        (*i)++;
        return NORMAL;
    }
    else if (expand_vars && string.s[*i] == '$')
    {
        count_var_len(string, i, len);
    }
    else
    {
        (*i)++;
        (*len)++;
    }
    return DQUOTE_STATE;
}

t_expansion_state count_squote_mode(t_string string, size_t *i, size_t *len)
{
    if (string.s[*i] == '\'')
    {
        (*i)++;
        return NORMAL;
    }
    else
    {
        (*i)++;
        (*len)++;
    }
    return SQUOTE_STATE;
}

size_t get_expanded_str_len(t_string string, int expand_vars)
{
    t_expansion_state state;
    size_t len;
    size_t i;

    len = 0;
    i = 0;
    state = NORMAL;
    while (i < string.count)
    {
        if (state == NORMAL)
            state = count_normal_mode(string, &i, &len, expand_vars);
        else if (state == DQUOTE_STATE)
            state = count_dquote_mode(string, &i, &len, expand_vars);
        else if (state == SQUOTE_STATE)
            state = count_squote_mode(string, &i, &len);
    }
    return len;
}