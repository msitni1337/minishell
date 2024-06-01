#include "interpreter.h"

int contains_chars(t_string string, char *charset)
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