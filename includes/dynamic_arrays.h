#ifndef DYNAMIC_ARRAYS_H
#define DYNAMIC_ARRAYS_H
#define DA_DEFAULT_CAPACITY 24

#include "common.h"

typedef struct s_arr
{
    void* data;
    size_t count;
    size_t capacity;
} t_arr;

t_arr* expand_arr(t_arr*arr, size_t size);
t_arr* add_to_arr(t_arr*arr, size_t size, void*data);

#endif