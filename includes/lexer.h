/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:43:22 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 11:37:14 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "environment.h"
# include "shared.h"
# include "shell.h"

# define SYN_ERR "syntax error."
# define SYN_QUOTE "unclosed quote."
# define SYN_BRACE "unclosed brace."

# define HERE_DOC_FILENAME_ERROR \
	"Can't get a valid here_doc file name after 100 tries.\n\
	Maybe it's time to clean up your /tmp folder manually."
# define REDIRECTION_FNAME_ERR \
"expecting name of file or here_doc delim to redirect I/O."
# define CMD_BEFORE_SUBSHL "CMD ARGS BEFORE SUBSHELL OR MULTIPLE SUBSHELLS"

typedef enum e_expansion_state
{
	NORMAL,
	DQUOTE_STATE,
	SQUOTE_STATE,
}		t_expansion_state;

// Tree Builder:
t_node	*create_node(int type);
void	init_root(t_node **root);
void	append_node(t_node **parent, t_node *child);
void	append_child(t_node *parent, t_node *child);
int		is_valid_redirect_parent(t_node_type type);
t_node	*add_str_node(t_node *root, t_lexer *lexer);
t_token	*fill_cmd(t_node **root, t_token *token, t_lexer *lexer, int as_child);

// Lexer:
t_node	**parse_line(char *line, t_node **root);

// Lexer_utils:
t_node	*get_node_by_type(t_node *root, t_node_type type);
t_node	*link_new_node(t_node **root, int type, int as_child);
t_node	**link_logic_oper(t_node **root, t_lexer *lexer, t_token *token,
			int as_child);
t_node	**init_subshell_node(t_node *root, t_lexer *lexer, t_token *token,
			t_node **subshell);
t_node	*link_argv_node(t_node *curr_cmd, t_lexer *lexer);
void	*syntax_error(char *reason);

// tokenizer:
t_token	get_next_token(t_lexer *lexer, int ignore_spaces);

// token_utils:
t_lexer	new_lexer(char *line);
void	trim_spaces(t_lexer *lexer);
int		is_special(char c);
int		is_cmd_token(t_token token);
int		is_redirect_token(t_token token);
int		is_logic_op(t_token token);

/* here_doc */
int		randomize_str(char *str, int rand_fd);
int		handle_here_doc_interrupt(int stdin_dup);
int		get_here_doc(t_node *node);

/* t_string utils */
int		contains_chars(t_string string, char *charset);

/* string expansion */
char	*expand_string(t_string string, int expand_vars);
size_t	get_expanded_str_len(t_string string, int expand_vars);
size_t	parse_key_count(const char *s);
void	copy_var_value(char *res, t_string *string, size_t *i);
int		count_num_chars(long n);

/* asterices expansion */
char	**expand_asterices(char **argv, size_t *argc);
size_t	get_expanded_args_count(char **argv, char **cwdfiles);
char	**get_all_cwd_filenames(void);
int		is_pattern_matching(char *pattern, char *name);
char	**copy_same_argument(char **res, size_t *index, char *argv,
			char **cwdfiles);
void	count_matching(char *argv, char **cwdfiles, size_t *res);
int		matching_loop(char **pattern, char *name, char *star, char *star_pos);

#endif