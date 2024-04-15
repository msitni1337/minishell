#include <stdio.h>

int main(int c, char**v)
{
    for (int i = 0; i<c;i++)
        printf("%s\n", v[i]);
}