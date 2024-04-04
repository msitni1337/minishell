#include "dynamic_arrays.h"

t_arr	*expand_arr(t_arr *arr, size_t size)
{
	void	*tmp;

	if (!arr || arr->count < arr->capacity)
		return arr;
	arr->capacity += DA_DEFAULT_CAPACITY;
	tmp = arr->data;
	arr->data = malloc(arr->capacity * size);
	if (!arr->data)
		return (free(tmp), NULL);
    if (arr->count)
        ft_memcpy(arr->data, tmp, size * arr->count);
	return (arr);
}

t_arr   *add_to_arr(t_arr *arr, size_t size, void *data)
{
    if (arr->count < arr->capacity && !expand_arr(arr, size))
        return NULL;
    ft_memcpy(arr->data + arr->count, data, size);
    arr->count++;
    return arr;
}