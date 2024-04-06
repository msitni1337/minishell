#ifndef GC_H
# define GC_H
# include "shared.h"

typedef struct s_mem {
    void*ptr;
    int is_freed;
    struct s_ptr*next;
}t_mem;

// TODO: append ptrs to current mem block
t_mem*get_current_mem();
void append_mem();

void*galloc(size_t size);
void*gfree(void*ptr);

//todo free current mem block
void*gcollect();

//todo free all memory blocks
void*gdestroy();

#endif