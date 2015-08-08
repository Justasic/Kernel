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
#include "mm/paging.h"
#include "mm/frame.h"
#include "mm/kmemory.h"
#include "mm/kheap.h"
#include "paging.h"
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

// Current kernel heap
extern heap_t *kheap;

void InitializePaging(void)
{
	// The size of physical memory. For the moment we
	// assume it is 16MB big.
	uint32_t mem_end_page = 0x1000000;
	
	nframes = mem_end_page / PAGE_SIZE;
	frames = kalloc(INDEX_FROM_BIT(nframes));
	
	// Let's make a page directory.
	kern_directory = kalloc_align(sizeof(page_directory_t));
	kern_directory->physicalAddr = (uint32_t)kern_directory->tablesPhysical;
	
	// Map some pages in the kernel heap area.
	// Here we call GetPage but not AllocFrame. This causes page_table_t's 
	// to be created where necessary. We can't allocate frames yet because they
	// they need to be identity mapped first below, and yet we can't increase
	// placement_addr between identity mapping and enabling the heap!
	uint32_t i = 0;
	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += PAGE_SIZE)
		GetPage(i, 1, kern_directory);
	
	// We need to identity map (phys addr = virt addr) from
	// 0x0 to the end of used memory, so we can access this
	// transparently, as if paging wasn't enabled.
	// NOTE: we use a while loop here deliberately.
	// inside the loop body we actually change placement_addr
	// by calling kalloc(). A while loop causes this to be
	// computed on-the-fly rather than once at the start.
	for (i = 0; i < placement_addr+PAGE_SIZE; i += PAGE_SIZE)
		// Kernel code is readable but not writeable from userspace.
		AllocFrame(GetPage(i, 1, kern_directory), 0, 0);
	
	// Now allocate those pages we mapped earlier.
	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += PAGE_SIZE)
		AllocFrame(GetPage(i, 1, kern_directory), 0, 0);

	// Register our paging interrupt handler.
	RegisterIRQHandler(PAGE_FAULT, ISRHandler);

	// Now, enable paging!
	SwitchPagingDirectory(kern_directory);
	
	// Initialize the kernel heap.
	kheap = CreateHeap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
	
	// Clone the kernel directory
	cur_directory = CloneDirectory(kern_directory);
	SwitchPagingDirectory(cur_directory);
}

void SwitchPagingDirectory(page_directory_t *dir)
{
	cur_directory = dir;
	uint32_t cr0;
	__asm__ __volatile__("mov %0, %%cr3":: "r"(dir->physicalAddr));
	__asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging bit
	__asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

void DisablePaging(void)
{
	uint32_t cr0;
	__asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
	cr0 &= ~0x80000000; // Disable paging bit
	__asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

void EnablePaging(void)
{
	uint32_t cr0;
	__asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging bit
	__asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

page_t *GetPage(uint32_t address, int make, page_directory_t *dir)
{
	// Turn the address into an index.
	address /= PAGE_SIZE;
	// Find the page table containing this address.
	uint32_t table_idx = address / 1024;
	
	if (dir->tables[table_idx]) // If this table is already assigned
		return &dir->tables[table_idx]->pages[address % 1024];
	else if(make)
	{
		uint32_t tmp;
		
		dir->tables[table_idx] = kalloc_align_phys_nowipe(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, PAGE_SIZE);
		dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, UM.
		
		return &dir->tables[table_idx]->pages[address % 1024];
	}
	else
		return 0;
}

static page_table_t *CloneTable(page_table_t *src, uint32_t *physAddr)
{
	// Make a new page table, which is page aligned.
	page_table_t *table = kalloc_align_phys(sizeof(page_table_t), physAddr);
	
	// For every entry in the table...
	for (int i = 0; i < 1024; i++)
	{
		// If the source entry has a frame associated with it...
		if (src->pages[i].frame)
		{
			// Get a new frame.
			AllocFrame(&table->pages[i], 0, 0);
			// Clone the flags from source to destination.
			if (src->pages[i].present)
				table->pages[i].present = 1;
			
			if (src->pages[i].rw)
				table->pages[i].rw = 1;
			
			if (src->pages[i].user)
				table->pages[i].user = 1;
			
			if (src->pages[i].accessed)
				table->pages[i].accessed = 1;
			
			if (src->pages[i].dirty)
				table->pages[i].dirty = 1;
			
// 			printf("Cloning ")
			// Physically copy the data across. This function is in kern/i386/paging.S.
			ClonePhysicalPage(src->pages[i].frame * PAGE_SIZE, table->pages[i].frame * PAGE_SIZE);
		}
	}
	return table;
}

page_directory_t *CloneDirectory(page_directory_t *src)
{
	uint32_t phys;
	
	// Make a new page directory and obtain its physical address
	page_directory_t *dir = kalloc_align_phys(sizeof(page_directory_t), &phys);
	
	// Get the offset of tablesPhysical from the start of the page_directory_t structure.
	uint32_t offset = (uint32_t)dir->tablesPhysical - (uint32_t)dir;
	
	// Then the physical address of dir->tablesPhysical is:
	dir->physicalAddr = phys + offset;
	
	// Go through each page table. If the page table is in the kernel directory, do not make a new copy.
	for (int i = 0; i < 1024; ++i)
	{
		if (!src->tables[i])
			continue;
		
		if (kern_directory->tables[i] == src->tables[i])
		{
			// It's in the kernel, so just use the same pointer
			dir->tables[i] = src->tables[i];
			dir->tablesPhysical[i] = src->tablesPhysical[i];
		}
		else
		{
			// Copy the table.
			uint32_t physaddr;
			dir->tables[i] = CloneTable(src->tables[i], &physaddr);
			dir->tablesPhysical[i] = physaddr | 0x07;
		}
	}
	
	return dir;
}
