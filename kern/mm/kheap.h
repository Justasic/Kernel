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
#include <stddef.h>
#include <stdbool.h>
#include "lib/oarray.h"

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x8BADF00D
#define HEAP_MIN_SIZE     0x70000

typedef struct
{
	uint32_t magic;    // Magic number, used for error checking and identification.
	bool is_hole;      // 1 if this is a hole. 0 if this is a block.
	bool is_clean;     // 1 if the hole was cleaned, 0 if it was not.
	uint32_t size;     // size of the block, including the end footer.
} header_t;

typedef struct
{
	uint32_t magic;     // Magic number, same as in header_t.
	header_t *header; // Pointer to the block header.
} footer_t;

typedef struct
{
	ordered_array_t index;
	uint32_t start_address; // The start of our allocated space.
	uint32_t end_address;   // The end of our allocated space. May be expanded up to max_address.
	uint32_t max_address;   // The maximum address the heap can be expanded to.
	bool supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
	bool readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

extern void hfree(void *p, heap_t *heap);
extern void *halloc(uint32_t size, uint8_t page_align, heap_t *heap);
extern heap_t *CreateHeap(uint32_t start, uint32_t end_addr, uint32_t max, bool supervisor, bool readonly);
extern void HeapCleanHoles(heap_t *heap);
