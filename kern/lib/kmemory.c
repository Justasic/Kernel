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

#include "lib/kmemory.h"
#include <stdint.h>
#include <stddef.h> // for NULL
#include <stdbool.h>
#include <string.h>

// Defined in kern/i386/pc/layout.ld
extern uint32_t bin_end;
// Our placement address to start allocations
uint32_t placement_addr = (uint32_t)&bin_end;

// Common function used for memory allocation.
static void *kalloc_int(size_t sz, bool align, uint32_t *phys)
{
	if (align == true && (placement_addr & 0xFFFFF000))
	{
		// Align the placement address
		placement_addr &= 0xFFFFF000;
		placement_addr += 0x1000;
	}
	
	if (phys)
		*phys = placement_addr;
	
	// allocate a new memory block
	uint32_t tmp = placement_addr;
	placement_addr += sz;
	
	// cast to a pointer
	void *ptr = (void*)tmp;
	// Null the memory space pointed to that pointer
// 	explicit_bzero(ptr, sz);
	
	// return our null-ed memory space
	return ptr;
}

// Returns a page-aligned pointer
void *kalloc_align(size_t sz)
{
	return kalloc_int(sz, true, NULL);
}

// Returns a physical address
void *kalloc_phys(size_t sz, uint32_t *phys)
{
	return kalloc_int(sz, false, phys);
}

// Returns a page-aligned pointer and physical address
void *kalloc_align_phys(size_t sz, uint32_t *phys)
{
	return kalloc_int(sz, true, phys);
}

// Just returns a pointer
void *kalloc(size_t sz)
{
	return kalloc_int(sz, false, NULL);
}

// This neat function reduces the memory size and applies the appropriate
// suffix to it. This is useful for printing debug messages and such.
uint32_t MemoryReduce(uint32_t bytes)
{
	uint32_t ret = bytes;
	while (ret < 1024)
		ret >>= 10;
	return ret;
}

const char *MemoryReduceSuffix(uint32_t bytes)
{
	const char *suffixes[] = {
		"B"
		"KB",
		"MB",
		"GB",
		"TB",
		"PB",
		"EB",
		"ZB",
		"YB",
		"Too damn big"
	};
	
	uint32_t ret = bytes, arr = 0;
	while(ret < 1024)
	{
		ret >>= 10;
		arr++;
	}
	
	if (ret > (sizeof(suffixes) / sizeof(*suffixes)))
		return "Too damn big";

	return suffixes[arr];
}
