#ifndef DYNAMIC_ARRAYS_H
#define DYNAMIC_ARRAYS_H
#define DA_DEFAULT_CAPACITY 24

#define ARR_AT(arr, type, i) (((type)(arr).data)[(i)])
#define ARR_LAST(arr, type) ((type)(arr).data)[(arr).count - 1]

#include "shared.h"

typedef struct s_arr
{
    void* data;
    size_t count;
    size_t elem_size;
    size_t capacity;
} t_arr;

t_arr* expand_arr(t_arr*arr);
t_arr* add_to_arr(t_arr*arr, void*data);
t_arr init_da(size_t size, void*elem);

#endif