#include "common.h"

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

void free_cmd(t_cmd cmd)
{
    free(cmd.bin_path);
    free_arr(cmd.args);
}