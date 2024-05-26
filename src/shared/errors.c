#include "error_handler.h"

void malloc_error(t_cmd*cmd)
{
    ft_putendl_fd(MALLOC_ERROR_MSG, STDERR_FILENO);
    free_tree(cmd->tree_root);
    free_global_vars();
    exit(EXIT_FAILURE);
}

void exit_with_code(t_cmd*cmd, int exit_value)
{
    free_tree(cmd->tree_root);
    free_global_vars();
    exit(exit_value);
}