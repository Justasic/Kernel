/*
 * Copyright (c) 2014, Justin Crawford <Justasic@gmail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright notice 
 * and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "oarray.h"
#include "lib/panic.h"
#include "mm/kmemory.h"
#include <string.h>
#include <stddef.h>

ordered_array_t CreateOrderedArray(uint32_t max_size, lessthan_predicate_t comp)
{
	ordered_array_t ret;
	ret.array = kalloc(max_size * sizeof(type_t));
	
	explicit_bzero(ret.array, max_size * sizeof(type_t));
	
	ret.size = 0;
	ret.max_size = max_size;
	ret.comp = comp;
	
	return ret;
}

ordered_array_t PlaceOrderedArray(void *addr, uint32_t max_size, lessthan_predicate_t comp)
{
	ordered_array_t ret;
	ret.array = (type_t*)addr;
	
	explicit_bzero(ret.array, max_size * sizeof(type_t));
	
	ret.size = 0;
	ret.max_size = max_size;
	ret.comp = comp;
	
	return ret;
}

void DestoryOrderedArray(ordered_array_t *array)
{
	// kfree(array->array);
}

void InsertOrderedArray(type_t item, ordered_array_t *array)
{
	if (!array->comp)
		panic(NULL, "array->comp was null!");
	
	uint32_t iterator = 0;
	while (iterator < array->size && array->comp(array->array[iterator], item))
		iterator++;
	
	if (iterator == array->size) // just add at the end of the array.
		array->array[array->size++] = item;
	else
	{
		type_t tmp = array->array[iterator];
		array->array[iterator] = item;
		
		while (iterator < array->size)
		{
			iterator++;
			type_t tmp2 = array->array[iterator];
			array->array[iterator] = tmp;
			tmp = tmp2;
		}
		
		array->size++;
	}
}

type_t LookupOrderedArray(uint32_t i, ordered_array_t *array)
{
	if (i < array->size)
		panic(NULL, "i < array->size: %d < %d\n", i, array->size);
	
	return array->array[i];
}

void RemoveOrderedArray(uint32_t i, ordered_array_t *array)
{
	while (i < array->size)
	{
		array->array[i] = array->array[i+1];
		i++;
	}
	
	array->size--;
} 
