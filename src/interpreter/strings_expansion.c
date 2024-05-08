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

size_t get_len(t_string string)
{
    (void) string;
    size_t len;
    len = 0;

    /*
        get the size of the resulted str after expanding all variables and removing quotes
    */

    return len;
}

char *perform_string_expansion(t_string string)
{
    char *res;
    size_t len;
    len = get_len(string);
    res = malloc(len + 1);

    /*
        do logic of expanding the string
    */

    return res;
}

char *expand_string(t_string string)
{
    if (contains_chars(string, "'\"$") == TRUE)
        return perform_string_expansion(string);
    else
        return ft_substr(string.s, 0, string.count);
}