#include "interpreter.h"

#define BUFF_SZ 1024

char *read_entire_stdin()
{
    t_darr arr;
    int bytes;

    arr.data = malloc(BUFF_SZ);
    arr.capacity = BUFF_SZ;
    arr.count = 0;
    while (1)
    {
        if (arr.count >= arr.capacity)
        {
            arr.capacity += BUFF_SZ;
            char *tmp = malloc(arr.capacity);
            ft_memcpy(tmp, arr.data, arr.count);
            free(arr.data);
            arr.data = tmp;
        }
        bytes = read(0, arr.data + arr.count, BUFF_SZ - 1);
        if (bytes < 0)
        {
            ((char *)arr.data)[arr.count] = 0;
            break;
        }
        else
        {
            arr.count += bytes;
            ((char *)arr.data)[arr.count] = 0;
            if (bytes == 0)
                break;
        }
    }
    return arr.data;
}

void execute_file()
{
    t_node *cmd_root;
    char *line;

    line = read_entire_stdin(0);
    if (parse_line(line, &cmd_root) != NULL)
    {
        shell.last_exit_value = interpret_root(cmd_root);
    }
    else
    {
        close_fds();
        shell.last_exit_value = 2;
    }

    free(line);
}