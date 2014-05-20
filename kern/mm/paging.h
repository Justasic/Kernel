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
#ifndef __KPAGING_H__
#define __KPAGING_H__

#include <stddef.h>
#include <stdint.h>
#include "lib/common.h"
#include "i386/pc/isr.h"

typedef struct
{
	uint32_t present    : 1;   // Page present in memory
	uint32_t rw         : 1;   // Read-only if clear, readwrite if set
	uint32_t user       : 1;   // Supervisor level only if clear
	uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
	uint32_t dirty      : 1;   // Has the page been written to since last refresh?
	uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
	uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct
{
	page_t pages[1024];
} page_table_t;

typedef struct
{
	// Array of pointers to pagetables.
	page_table_t *tables[1024];
	// Array of pointers to the pagetables above, but gives their *physical*
	// location, for loading into the CR3 register.
	uint32_t tablesPhysical[1024];
	// The physical address of tablesPhysical. This comes into play
	// when we get our kernel heap allocated and the directory
	// may be in a different location in virtual memory.
	uint32_t physicalAddr;
} page_directory_t;

extern page_directory_t *kern_directory;
extern page_directory_t *cur_directory;

// Sets up the environment, page directories etc and enables paging.
extern void initialize_paging(void);

// Causes the specified page directory to be loaded into the CR3 register.
extern void SwitchPagingDirectory(page_directory_t *new);

// Retrieves a pointer to the page required. If make == 1, if the page-table in which
// this page should reside isn't created, create it!
extern page_t *GetPage(uint32_t address, int make, page_directory_t *dir);

// Disables paging so we can do things which may cause page faults but are considered valid
extern void DisablePaging(void);

#endif // __KPAGING_H__
