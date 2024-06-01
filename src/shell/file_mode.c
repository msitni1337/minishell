#include "interpreter.h"

char *read_entire_stdin()
{
    char buff[BUFF_SZ];
    char *res;
    char *tmp;
    int bytes;

    res = NULL;
    bytes = read(STDIN_FILENO, buff, BUFF_SZ - 1);
    while (bytes > 0)
    {
        buff[bytes] = 0;
        tmp = ft_strjoin(res, buff);
        if (tmp == NULL)
            malloc_error(res, NULL, NULL, NULL);
        free(res);
        res = tmp;
        bytes = read(STDIN_FILENO, buff, BUFF_SZ - 1);
    }
    return res;
}

void execute_file()
{
    shell.line = read_entire_stdin();
    if (parse_line(shell.line, &shell.tree_root) != NULL)
    {
        shell.last_exit_value = interpret_root(shell.tree_root);
    }
    else
    {
        shell.last_exit_value = 2;
        if (shell.interrupt == TRUE)
        {
            shell.interrupt = FALSE;
            close_here_docs();
            shell.last_exit_value = 130;
        }
    }
    exit_with_code(NULL, shell.last_exit_value);
}