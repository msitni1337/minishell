#include <unistd.h>
#include <stdio.h>

#define BUFF_SZ 1024
int main(int c, char **v, char **envp)
{
    int i;
    printf("envp:\n");
    i = 0;
    while (envp && envp[i])
    {

        printf("%s\n", envp[i]);
        i++;
    }
    printf("Args:\n");
    for (int i = 0; i < c; i++)
        printf("%d => %s\n", i, v[i]);

    printf("infile content:\n");
    lseek(0, 0, SEEK_SET);
    char buff[BUFF_SZ];
    int bytes;
    while ((bytes = read(0, buff, BUFF_SZ - 1)) > 0)
    {
        buff[bytes] = 0;
        printf("%s", buff);
    }
    return 0;
}
