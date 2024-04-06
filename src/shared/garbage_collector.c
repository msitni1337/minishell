#include "gc.h"

typedef struct s_ptr {
    void*ptr;
    int is_freed;
    struct s_ptr*next;
}t_ptr;

s_ptr*get_root
void*galloc(size_t size);
void*gfree(void*ptr);
void*gcollect();