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
#include "lib/paging.h"
#include "lib/frame.h"
#include "lib/kmemory.h"
#include <string.h>
#include <stdio.h>

// defined in lib/frame.c
extern uint32_t *frames;
extern uint32_t nframes;

// defined in lib/kmemory.c
extern uint32_t placement_addr;

// Kernel paging directory
page_directory_t *kern_directory = 0;

// Current paging directory
page_directory_t *cur_directory = 0;

void initialize_paging(void)
{
	// The size of physical memory. For the moment we
	// assume it is 16MB big.
	uint32_t mem_end_page = 0x1000000;
	
	nframes = mem_end_page / 0x1000;
	frames = kalloc(INDEX_FROM_BIT(nframes));
	
	// Let's make a page directory.
	kern_directory = kalloc_align(sizeof(page_directory_t));
	cur_directory = kern_directory;
	
	// We need to identity map (phys addr = virt addr) from
	// 0x0 to the end of used memory, so we can access this
	// transparently, as if paging wasn't enabled.
	// NOTE: we use a while loop here deliberately.
	// inside the loop body we actually change placement_addr
	// by calling kalloc(). A while loop causes this to be
	// computed on-the-fly rather than once at the start.
	uint32_t i = 0;
	while (i < placement_addr)
	{
		// Kernel code is readable but not writeable from userspace.
		alloc_frame(get_page(i, 1, kern_directory), 0, 0);
		i += 0x1000;
	}
	
	printf("Initializing paging\n");

	// Now, enable paging!
	switch_page_directory(kern_directory);
}

void switch_page_directory(page_directory_t *dir)
{
	cur_directory = dir;
	uint32_t cr0;
	__asm__ __volatile__("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
	__asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging bit
	__asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
	// Turn the address into an index.
	address /= 0x1000;
	// Find the page table containing this address.
	uint32_t table_idx = address / 1024;
	
	if (dir->tables[table_idx]) // If this table is already assigned
		return &dir->tables[table_idx]->pages[address%1024];
	else if(make)
	{
		uint32_t tmp;
		dir->tables[table_idx] = kalloc_align_phys(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, UM.
		return &dir->tables[table_idx]->pages[address % 1024];
	}
	else
		return 0;
} 
