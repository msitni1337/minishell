/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:42:43 by msitni            #+#    #+#             */
/*   Updated: 2024/06/04 13:34:01 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H
# include "dynamic_arrays.h"
# include "interpreter.h"
# include <signal.h>

typedef struct s_shell
{
	t_lstenv	*env_list;
	t_node		*tree_root;
	char		*line;
	int			last_exit_value;
	t_darr		childs_pids;
	t_darr		here_docs;
	int			interrupt;
	int			collecting_here_doc;
}				t_shell;

// global variable:
extern t_shell	g_shell;

/* init */
void			init_shell(const char **envp);
void			setup_signal_handlers(void);

/* shell mode */
void			start_shell(void);

/* file mode */
void			execute_from_stdin(void);

/* prompt */
char			*get_prompt(void);

#endif