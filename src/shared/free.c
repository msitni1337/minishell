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

void close_fds()
{
    struct stat s; 
    int fd;

    fd = 3;
    while (fd > 2 && fstat(fd, &s) != -1 && errno != EBADF)
    {
        close(fd);
        fd++;
    }
}