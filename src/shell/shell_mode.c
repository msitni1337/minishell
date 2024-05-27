#include "interpreter.h"

void print_tree(t_node *root);

char *get_prompt()
{
    char *home;
    char *tmp;
    char *res;

    tmp = getcwd(NULL, 0);
    home = get_env_value("HOME");
    if (ft_strncmp(tmp, home, ft_strlen(home)) == 0)
    {
        res = ft_strjoin(PROMPTSTART, "~");
        home = ft_strjoin(res, tmp + ft_strlen(home));
        free(tmp);
        tmp = ft_strjoin(home, PROMPTEND);
        free(home);
        res = tmp;
    }
    else
    {
        res = ft_strjoin(PROMPTSTART, tmp);
        free(tmp);
        tmp = ft_strjoin(res, PROMPTEND);
        free(res);
        res = tmp;
    }
    return res;
}

void assert_all_files_closed()
{
    struct stat s;
    int fd;

    fd = 3;
    while (fd > 2 && fstat(fd, &s) != -1 && errno != EBADF)
    {
        fd++;
    }
    assert(fd == 3 && "FILES NOT CLOSED PROPERLY");
}

void start_shell()
{
    char *prompt;

    shell.interrupt = FALSE;
    shell.collecting_here_doc = FALSE;
    prompt = get_prompt();
    shel.line = readline(prompt);
    free(prompt);
    while (shell.line)
    {
        shell.interrupt = FALSE;
        add_history(shell.line);
        if (parse_line(shell.line, &shell.tree_root) != NULL)
        {
            /*
            if (cmd_root)
                print_tree(cmd_root);
            readline("PRESS ENTER TO EXECUTE TREE");
            */
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
        free_tree(&shell.tree_root);
        assert_all_files_closed();
        free(shell.line);
        shell.line = NULL;
        prompt = get_prompt();
        shell.line = readline(prompt);
        free(prompt);
    }
}