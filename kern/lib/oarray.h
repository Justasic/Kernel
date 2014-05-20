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
#pragma once

#include <stdint.h>

// Generic type container
typedef void* type_t;

// A predicate should return nonzero if the first argument is less than the second.
// otherwise it should return zero.
typedef int8_t (*lessthan_predicate_t)(type_t,type_t);
typedef struct
{
   type_t *array;
   uint32_t size;
   uint32_t max_size;
   lessthan_predicate_t comp;
} ordered_array_t;

ordered_array_t CreateOrderedArray(uint32_t max_size, lessthan_predicate_t comp);
ordered_array_t PlaceOrderedArray(void *addr, uint32_t max_size, lessthan_predicate_t comp);

void DestoryOrderedArray(ordered_array_t *array);
void InsertOrderedArray(type_t item, ordered_array_t *array);
type_t LookupOrderedArray(uint32_t i, ordered_array_t *array);
void RemoveOrderedArray(uint32_t i, ordered_array_t *array);
