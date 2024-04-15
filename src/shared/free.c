#include "shared.h"

void free_arr(char**arr)
{
    char**tmp;

    tmp = arr;
    while (arr && *arr)
    {
        free(*arr);
        arr++;
    }
    if (arr)
        free(tmp);
}