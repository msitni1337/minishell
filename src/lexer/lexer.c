/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:10:06 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:10:06 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*parse_subshell(t_node *root, t_lexer *lexer, t_token *token)
{
	t_node	*subshell;

	if (init_subshell_node(root, lexer, token, &subshell) == NULL)
		return (NULL);
	while (token->type != TOKEN_EOF)
	{
		if (is_cmd_token(*token))
		{
			if (fill_cmd(&subshell, token, lexer, TRUE) != NULL)
				continue ;
			return (NULL);
		}
		else if (subshell->childs_count == 0)
			return (syntax_error("empty subshell."));
		else if (is_logic_op(*token))
		{
			if (link_logic_oper(&subshell, lexer, token, TRUE))
				continue ;
			return (syntax_error(SYN_ERR));
		}
		else if (token->type == TOKEN_CLOSE_PAREN)
			break ;
		*token = get_next_token(lexer, TRUE);
	}
	return (token);
}

int	add_redirect_node(t_lexer *lexer, t_node *curr_cmd, int type)
{
	t_token	token;
	t_node	*tmp;

	tmp = create_node(type);
	if (tmp == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	append_child(curr_cmd, tmp);
	token = get_next_token(lexer, TRUE);
	if (token.type == TOKEN_STR)
	{
		tmp = add_str_node(tmp, lexer);
		if (tmp == NULL)
		{
			syntax_error(SYN_QUOTE);
			return (1);
		}
		if (type == NODE_HERE_DOC && get_here_doc(tmp))
			return (1);
	}
	else
	{
		syntax_error(REDIRECTION_FNAME_ERR);
		return (1);
	}
	return (0);
}

t_token	*fill_cmd(t_node **root, t_token *token, t_lexer *lexer, int as_child)
{
	t_node	*curr_cmd;
	int		can_have_subshell;

	can_have_subshell = TRUE;
	curr_cmd = link_new_node(root, NODE_CMD, as_child);
	while (is_cmd_token(*token))
	{
		if (token->type == TOKEN_STR && link_argv_node(curr_cmd, lexer) == NULL)
			return (NULL);
		else if (is_redirect_token(*token) && add_redirect_node(lexer,
		 		curr_cmd, token->type))
			return (NULL);
		else if (token->type == TOKEN_OPEN_PAREN)
		{
			if (can_have_subshell == FALSE)
				return (syntax_error(CMD_BEFORE_SUBSHL));
			if (parse_subshell(curr_cmd, lexer, token) == NULL)
				return (NULL);
			if (token->type != TOKEN_CLOSE_PAREN)
				return (syntax_error(SYN_BRACE));
		}
		can_have_subshell = FALSE;
		*token = get_next_token(lexer, TRUE);
	}
	return (token);
}

t_node	**parser_loop(t_node **root, t_lexer *lexer, t_token *token)
{
	*root = NULL;
	g_shell.here_docs_count = 0;
	while (token->type != TOKEN_EOF)
	{
		if (is_cmd_token(*token))
		{
			token = fill_cmd(root, token, lexer, FALSE);
			if (token)
				continue ;
			return (NULL);
		}
		if ((*root)->list_count > 0)
		{
			if (is_logic_op(*token))
			{
				if (link_logic_oper(root, lexer, token, FALSE))
					continue ;
			}
			return (syntax_error(SYN_ERR));
		}
		else
			return (syntax_error(SYN_ERR));
		*token = get_next_token(lexer, TRUE);
	}
	return (root);
}

t_node	**parse_line(char *line, t_node **root)
{
	t_lexer	lexer;
	t_token	token;

	lexer = new_lexer(line);
	token = get_next_token(&lexer, TRUE);
	if (token.type == TOKEN_EOF)
		return (NULL);
	if (!is_cmd_token(token))
		return (syntax_error(SYN_ERR));
	return (parser_loop(root, &lexer, &token));
}
