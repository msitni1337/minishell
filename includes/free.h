/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:43:29 by msitni            #+#    #+#             */
/*   Updated: 2024/06/02 09:43:31 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H
# include "lexer.h"

void	free_tree(t_node **root);
void	free_cmd(t_cmd *cmd);
void	*free_p(void *p1, void *p2, void *p3, char **p4);

#endif