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

void start_shell()
{
    t_node *cmd_root;
    char *line;
    char *prompt;

    prompt = get_prompt();
    line = readline(prompt);
    free(prompt);
    while (line != NULL)
    {
        add_history(line);
        cmd_root = parse_line(line);

        /*
                if (cmd_root)
                    print_tree(cmd_root);
                readline("PRESS ENTER TO EXECUTE TREE");
        */

        if (cmd_root)
            shell.last_exit_value = interpret_root(cmd_root);

        free(line);
        prompt = get_prompt();
        line = readline(prompt);
        free(prompt);
    }
}