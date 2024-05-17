#include "dynamic_arrays.h"

t_darr init_da(size_t elem_size, void*elem)
{
	t_darr res;

	ft_memset(&res, 0, sizeof(t_darr));
	res.elem_size = elem_size;
	expand_arr(&res);
	if (elem)
		add_to_arr(&res, elem);
	return res;
}

t_darr *expand_arr(t_darr *arr)
{
	void *tmp;

	if (!arr || arr->count < arr->capacity)
		return arr;
	arr->capacity += DA_DEFAULT_CAPACITY;
	tmp = arr->data;
	arr->data = malloc(arr->capacity * arr->elem_size); // todo : galloc..
	if (tmp && arr->count)
		ft_memcpy(arr->data, tmp, arr->elem_size * arr->count);
	free(tmp);
	return (arr);
}

t_darr *add_to_arr(t_darr *arr, void *data)
{
	if (arr->count >= arr->capacity)
		expand_arr(arr);
	ft_memcpy(((unsigned char*)arr->data) + arr->elem_size * arr->count, data, arr->elem_size);
	arr->count++;
	return arr;
}