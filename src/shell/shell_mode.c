#include "interpreter.h"

void print_tree(t_node *root);

char* get_chopped_prompt(char*cwd, int home_len)
{
    char *res;
    char *tmp;

    tmp = ft_strjoin(PROMPTSTART"~", cwd + home_len);
    free(cwd);
    if  (tmp == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
    res = ft_strjoin(tmp, PROMPTEND);
    free(tmp);
    if (res == NULL)
        malloc_error(cwd, NULL, NULL, NULL);
    return res;
}

char* get_full_prompt(char*cwd)
{
    char *res;
    char *tmp;

    tmp = ft_strjoin(PROMPTSTART, cwd);
    free(cwd);
    if (tmp == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
    res = ft_strjoin(tmp, PROMPTEND);
    free(tmp);
    if (res == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
    return res;
}

char *get_prompt()
{
    char *home;
    char *cwd;
    int home_len;

    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        malloc_error(NULL, NULL, NULL, NULL);
    home = get_env_value("HOME");
    home_len = ft_strlen(home);
    if (home && ft_strncmp(cwd, home, home_len) == 0)
        return get_chopped_prompt(cwd, home_len);
    else
        return get_full_prompt(cwd);
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

void add_line_to_hist(char*line)
{
    t_lexer lexer;
    t_token token;

    lexer = new_lexer(line);
    token = get_next_token(&lexer, TRUE);
    if (token.type != TOKEN_EOF)
        add_history(line);
}

void start_shell()
{
    char *prompt;

    shell.interrupt = FALSE;
    shell.collecting_here_doc = FALSE;
    prompt = get_prompt();
    shell.line = readline(prompt);
    free(prompt);
    while (shell.line)
    {
        shell.interrupt = FALSE;
        add_line_to_hist(shell.line);
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
        free_tree(&shell.tree_root);
        assert_all_files_closed();
        free(shell.line);
        shell.line = NULL;
        prompt = get_prompt();
        shell.line = readline(prompt);
        free(prompt);
    }
}