#include "dynamic_arrays.h"

t_darr init_da(size_t elem_size)
{
	t_darr res;

	res.elem_size = elem_size;
	res.capacity = 0;
	res.count = 0;
	res.data = NULL;
	return res;
}

t_darr *expand_arr(t_darr *arr)
{
	void *tmp;

	if (!arr || arr->count < arr->capacity)
		return NULL;
	arr->capacity += DA_DEFAULT_CAPACITY;
	tmp = malloc(arr->capacity * arr->elem_size);
	if (tmp == NULL)
		return NULL;
	ft_memset(tmp, 0, arr->elem_size * arr->capacity);
	if (arr->data && arr->count)
		ft_memcpy(tmp, arr->data, arr->elem_size * arr->count);
	free(arr->data);
	arr->data = tmp;
	return (arr);
}

t_darr *add_to_arr(t_darr *arr, void *data)
{
	if (arr->count >= arr->capacity)
	{
		if (expand_arr(arr) == NULL)
			return NULL;
	}
	ft_memcpy(((unsigned char *)arr->data) + arr->elem_size * arr->count, data, arr->elem_size);
	arr->count++;
	return arr;
}