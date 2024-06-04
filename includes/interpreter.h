/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:43:17 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 20:11:39 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "built_ins.h"
# include "free.h"
# include "lexer.h"
# include "shared.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

//  interpreter:
int		interpret_root(t_node *root);
int		parse_cmd(t_node *node, t_cmd *cmd);

/* piping */
int		execute_piping(t_node **node, t_cmd *cmd);

//  execution:
int		execute_cmd(t_cmd *cmd, int is_pipe, int wait_child);
int		wait_all_childs(void);
int		exec_bin(t_cmd *cmd, int wait_child);
int		exec_builtin(t_cmd *cmd);
int		exec_builtin_fork(t_cmd *cmd);
int		exec_subshell(t_cmd *cmd, int wait_child);

/* reirections */
int		open_files(t_node *cmd_node, t_cmd *cmd);

/* argv */
void	get_argv(t_node *cmd_node, t_cmd *cmd);

/* bin path joining */
int		get_cmd_path(t_cmd *cmd);

/* utils */
t_node	*get_next_node_by_type(t_node *root, t_node_type type);
void	get_perm_flags(int *p_flags, int *m_flags, t_node_type type);
int		is_builtin(const char *s);

/* errors */
void	print_error(char *name, char *reason);

#endif