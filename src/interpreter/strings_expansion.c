#include "interpreter.h"

t_expansion_state expand_normal_mode(char *res, t_string *string, size_t *i, int expand_vars)
{
    if (*(string->s) == '\'')
    {
        (string->count)--;
        (string->s)++;
        return SQUOTE_STATE;
    }
    else if (*(string->s) == '"')
    {
        (string->count)--;
        (string->s)++;
        return DQUOTE_STATE;
    }
    else if (expand_vars && *(string->s) == '$')
    {
        copy_var_value(res, string, i);
    }
    else
    {
        res[*i] = *(string->s);
        (string->count)--;
        (string->s)++;
        (*i)++;
    }
    return NORMAL;
}

t_expansion_state expand_dquote_mode(char *res, t_string *string, size_t *i, int expand_vars)
{
    if (*(string->s) == '"')
    {
        (string->count)--;
        (string->s)++;
        return NORMAL;
    }
    else if (expand_vars && *(string->s) == '$')
    {
        copy_var_value(res, string, i);
    }
    else
    {
        res[*i] = *(string->s);
        (string->count)--;
        (string->s)++;
        (*i)++;
    }
    return DQUOTE_STATE;
}

t_expansion_state expand_squote_mode(char *res, t_string *string, size_t *i)
{
    if (*(string->s) == '\'')
    {
        (string->count)--;
        (string->s)++;
        return NORMAL;
    }
    else
    {
        res[*i] = *(string->s);
        (string->count)--;
        (string->s)++;
        (*i)++;
    }
    return SQUOTE_STATE;
}

char *perform_string_expansion(t_string string, int expand_vars)
{
    t_expansion_state state;
    char *res;
    size_t len;
    size_t i;

    len = get_expanded_str_len(string, expand_vars);
    res = malloc(len + 1);
    if (res == NULL)
        return NULL;
    i = 0;
    state = NORMAL;
    while (string.count > 0)
    {
        if (state == NORMAL)
            state = expand_normal_mode(res, &string, &i, expand_vars);
        else if (state == DQUOTE_STATE)
            state = expand_dquote_mode(res, &string, &i, expand_vars);
        else if (state == SQUOTE_STATE)
            state = expand_squote_mode(res, &string, &i);
    }
    res[i] = 0;
    return res;
}

char *expand_string(t_string string, int expand_vars)
{
    if (contains_chars(string, "*'\"$") == TRUE)
        return perform_string_expansion(string, expand_vars);
    else
        return ft_substr(string.s, 0, string.count);
}