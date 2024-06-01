#ifndef FREE_H
# define FREE_H
# include "lexer.h"

void	free_tree(t_node **root);
void	free_cmd(t_cmd *cmd);
void	*free_p(void *p1, void *p2, void *p3, char **p4);

#endif