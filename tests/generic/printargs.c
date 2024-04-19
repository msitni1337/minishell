#include <unistd.h>
#include <stdio.h>

#define BUFF_SZ 1024
int main(int c, char **v)
{
    printf("Args:\n");
    for (int i = 0; i < c; i++)
        printf("%d => %s\n", i, v[i]);

    printf("infile content:\n");
    char buff[BUFF_SZ];
    int bytes;
    while ((bytes = read(0, buff, BUFF_SZ - 1)) > 0)
    {
        buff[bytes] = 0;
        printf("%s", buff);
    }
}