#ifndef GC_H
# define GC_H
# include "shared.h"

typedef int t_bool;

# define TRUE 1
# define FALSE 0


typedef struct s_mem {
    void*ptr;
    t_bool is_freed;
	t_bool is_perm;
    struct s_mem*next;
}t_mem;

void	*gc_malloc(size_t size, t_bool is_perm);
void	gc_free(void *ptr);
void	gc_free_all(void);
void	gc_exit(int exit_status);

t_mem	**get_mem(void);
t_mem	*create_mem(void *ptr, t_bool is_perm);
void	add_end(t_mem **mem, t_mem *new);
t_mem	*check_for_place(t_mem *mem, void *ptr);
void	free_mem(t_mem *mem);


#endif