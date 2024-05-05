#include "lexer.h"

int get_string_whitespace(t_lexer *lexer, t_string *s)
{
    while (lexer->pos < lexer->count)
    {
        if (ft_isspace(lexer->line[lexer->pos]) || lexer->line[lexer->pos] == DQUOTE || lexer->line[lexer->pos] == SQUOTE || is_special(lexer->line[lexer->pos]))
            break;
        s->count++;
        lexer->pos++;
    }
    return TRUE;
}

int get_string_delim(t_lexer *lexer, t_string *s, const char delim)
{
    int is_closed;

    is_closed = FALSE;
    while (lexer->pos < lexer->count && !is_closed)
    {
        if (lexer->line[lexer->pos] == delim)
            is_closed = TRUE;
        s->count++;
        lexer->pos++;
    }
    return is_closed;
}

int get_string(t_lexer *lexer, t_string *s)
{
    int is_closed;

    if (*(s->s) == DQUOTE)
        is_closed = get_string_delim(lexer, s, DQUOTE);
    else if (*(s->s) == SQUOTE)
        is_closed = get_string_delim(lexer, s, SQUOTE);
    else
        is_closed = get_string_whitespace(lexer, s);
    while (is_closed && s->s[s->count] && !ft_isspace(s->s[s->count]) && !is_special(s->s[s->count]))
    {
        if (s->s[s->count] == DQUOTE)
            is_closed = get_string_delim(lexer, s, DQUOTE);
        else if (s->s[s->count] == SQUOTE)
            is_closed = get_string_delim(lexer, s, SQUOTE);
        else
            is_closed = get_string_whitespace(lexer, s);
    }
    return is_closed;
}

t_node *add_str_node(t_node *root, t_lexer *lexer)
{
    t_node *node;

    node = create_node(NODE_STRING);
    node->token_str.count = 1;
    node->token_str.s = &(lexer->line[lexer->pos - 1]);
    if (get_string(lexer, &(node->token_str)) == FALSE)
        return NULL;
    append_child(root, node);
    return node;
}