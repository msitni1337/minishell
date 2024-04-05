#include "lexer.h"

t_string get_string_whitespace(t_lexer *lexer)
{
    t_string res;
    int escape;

    res.s = &(lexer->line[lexer->pos - 1]);
    res.count = 1;
    escape = FALSE;
    while (lexer->pos < lexer->count)
    {
        if (!escape && ft_isspace(lexer->line[lexer->pos]))
            break;
        if (lexer->line[lexer->pos] == '\\')
            escape = !escape;
        else
            escape = FALSE;
        res.count++;
        lexer->pos++;
    }
    return res;
}

t_string get_string_delim(t_lexer *lexer, const char delim)
{
    t_string res;
    int escape;

    res.s = &(lexer->line[lexer->pos]);
    res.count = 0;
    escape = FALSE;
    while (lexer->pos < lexer->count)
    {
        if (!escape && lexer->line[lexer->pos] == delim)
            break;
        if (lexer->line[lexer->pos] == '\\')
            escape = !escape;
        else
            escape = FALSE;
        res.count++;
        lexer->pos++;
    }
    if (lexer->line[lexer->pos] == delim)
		{
     	lexer->pos++;
			return res;
		}
		return res; // todo => need to throw error "syntax error"
}

t_string get_dquote_string(t_lexer *lexer)
{
    t_token token;
    t_string res;

    USED(token);

    res = get_string_delim(lexer, DQUOTE);
    /* MSG 1
        This is for Nmellal (Hope You're good :):
        here gonna be code for handling string expansion such as env_var
        replacing => cmd $VAR1 $VAR2 and escaping chars eg ==> \
        now we're returning just the parsed str..
        ...
    */

    return res;
}

t_string get_squote_string(t_lexer *lexer)
{
    t_token token;
    t_string res;

    USED(token);
    res = get_string_delim(lexer, SQUOTE);

    /* MSG 2 same as MSG 1 with some special cases
        bla bla bla more code
        ...
    */

    return res;
}

void parse_line(char *line)
{
    t_lexer lexer;
    t_token token;
    t_string str;

    lexer = new_lexer(line);
    token = get_next_token(&lexer, TRUE);
    // To crash program if invalid token.. just fot testing purposes
    assert(token.type != TOKEN_INVALID);
    while (token.type != TOKEN_EOF)
    {
        assert(token.type != TOKEN_INVALID);
        if (token.type == TOKEN_DQUOTE)
        {
            ft_putstr_fd("Token = DQUOTE, text = [", 1);
            str = get_dquote_string(&lexer);
            write(1, str.s, str.count);
            ft_putendl_fd("]\n", 1);
        }
        else if (token.type == TOKEN_SQUOTE)
        {
            ft_putstr_fd("Token = SQUOTE, text = [", 1);
            str = get_squote_string(&lexer);
            write(1, str.s, str.count);
            ft_putendl_fd("]\n", 1);
        }
        /*
        else if (token.type == TOKEN_PIPE)
        {
            cmd_pipe(NOT IMPLEMENTED);
        }
        else if (token.type == TOKEN_REDIRECT_IN)
        {
            cmd_set_infile(NOT IMPLEMENTED);
        }
        else if (token.type == TOKEN_REDIRECT_OUT)
        {
            cmd_set_outfile(NOT IMPLEMENTED);
        }
        else if (token.type == TOKEN_HERE_DOC)
        {
            cmd_set_heredoc(NOT IMPLEMENTED);
        }
        else if (token.type == TOKEN_APPEND)
        {
            cmd_set_append(NOT IMPLEMENTED);
        }
        */
        else if (token.type == TOKEN_CHAR)
        {
            ft_putstr_fd("Token = CHAR, text = [", 1);
            str = get_string_whitespace(&lexer);
            /*TODO:
                Here we need to check for string expansions too,
                But depending on the context I choosed to delay implementing
                this part until the structure of the code becomes more clear
                then I'll choose the best place to inject it ..
            */
            write(1, str.s, str.count);
            ft_putendl_fd("]\n", 1);
        }
        token = get_next_token(&lexer, TRUE);
    }
}
