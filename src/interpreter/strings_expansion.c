#include "interpreter.h"

bool contains_chars(t_string string, char *charset)
{
    size_t i;
    size_t j;
    size_t charset_len;

    charset_len = ft_strlen(charset);
    i = 0;
    while (i < string.count)
    {
        j = 0;
        while (j < charset_len)
        {
            if (string.s[i] == charset[j])
                return TRUE;
            j++;
        }
        i++;
    }
    return FALSE;
}

#define eq(c1, c2) ((c1) == (c2))

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

size_t get_len(t_string string)
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
        {
            if (string.s[i] == '\'')
            {
                state = SQUOTE_STATE;
                i++;
                continue;
            }
            else if (string.s[i] == '"')
            {
                state = DQUOTE_STATE;
                i++;
                continue;
            }
            else if (string.s[i] == '$')
            {
                i++;
                int count = parse_key_count(string.s + i);
                if (count)
                {
                    len += ft_strlen(get_env_value(shell.env_list, ft_substr(string.s + i, 0, count)));
                    i += count;
                }
                continue;
            }
            i++;
            len++;
        }
        else if (state == DQUOTE_STATE)
        {
            if (string.s[i] == '"')
            {
                state = NORMAL;
                i++;
                continue;
            }
            else if (string.s[i] == '$')
            {
                i++;
                int count = parse_key_count(string.s + i);
                if (count)
                {
                    len += ft_strlen(get_env_value(shell.env_list, ft_substr(string.s + i, 0, count)));
                    i += count;
                }
                continue;
            }
            i++;
            len++;
        }
        else if (state == SQUOTE_STATE)
        {
            if (string.s[i] == '\'')
            {
                state = NORMAL;
                i++;
                continue;
            }
            i++;
            len++;
        }
        else
            assert(!"IMPOSSIBLE");
    }
    return len;
}

char *perform_string_expansion(t_string string)
{
    t_expansion_state state;
    char *res;
    size_t len;
    size_t i;
    size_t j;

    len = get_len(string);
    res = malloc(len + 1);
    state = NORMAL;
    i = 0;
    j = 0;

    while (i < string.count)
    {
        if (state == NORMAL)
        {
            if (string.s[i] == '\'')
            {
                state = SQUOTE_STATE;
                i++;
                continue;
            }
            else if (string.s[i] == '"')
            {
                state = DQUOTE_STATE;
                i++;
                continue;
            }
            else if (string.s[i] == '$')
            {
                i++;
                int count = parse_key_count(string.s + i);
                if (count)
                {
                    if (get_env_value(shell.env_list, ft_substr(string.s + i, 0, count)))
                        j += ft_strlcpy(res + j, get_env_value(shell.env_list, ft_substr(string.s + i, 0, count)), len - j + 1);
                    i += count;
                }
                else
                {
                    res[j] = '$';
                    j++;
                }
                continue;
            }
            else
            {
                res[j] = string.s[i];
                i++;
                j++;
            }
        }
        else if (state == DQUOTE_STATE)
        {
            if (string.s[i] == '"')
            {
                state = NORMAL;
                i++;
                continue;
            }
            else if (string.s[i] == '$')
            {
                i++;
                int count = parse_key_count(string.s + i);
                if (count)
                {
                    if (get_env_value(shell.env_list, ft_substr(string.s + i, 0, count)))
                        j += ft_strlcpy(res + j, get_env_value(shell.env_list, ft_substr(string.s + i, 0, count)), len - j + 1);
                    i += count;
                }
                else
                {
                    res[j] = '$';
                    j++;
                }
                continue;
            }
            else
            {
                res[j] = string.s[i];
                i++;
                j++;
            }
        }
        else if (state == SQUOTE_STATE)
        {
            if (string.s[i] == '\'')
            {
                state = NORMAL;
                i++;
                continue;
            }
            else
            {
                res[j] = string.s[i];
                i++;
                j++;
            }
        }
        else
            assert(!"IMPOSSIBLE");
    }
    res[j] = 0;
    return res;
}

char *expand_string(t_string string)
{
    if (contains_chars(string, "'\"$") == TRUE)
        return perform_string_expansion(string);
    else
        return ft_substr(string.s, 0, string.count);
}