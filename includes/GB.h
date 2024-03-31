#ifndef GB_H
#define GB_H
#include <stddef.h>

typedef struct s_mem {
    int is_freed;
    void* ptr;
    struct s_mem* next;
} t_mem;

t_mem* GB_get_mem_head();
void GB_init();
t_mem* GB_new(void* ptr);
void* GB_malloc(size_t size);
void GB_free(void* ptr);
void GB_free_all(void* ptr);

#endif