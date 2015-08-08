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
#include "mm/kheap.h"
#include "mm/paging.h"
#include "mm/frame.h"
#include "mm/kmemory.h"
#include "lib/oarray.h"
#include "lib/panic.h"

static void Expand(uint32_t sz, heap_t *heap)
{
	if (sz > heap->end_address - heap->start_address)
		panic(NULL, "Heap asked to expand %d bytes but only %d bytes available!",
		      sz, heap->end_address - heap->start_address);
	
	// Get the nearest following page boundary.
	if (sz & 0xFFFFF000)
	{
		sz &= 0xFFFFF000;
		sz += PAGE_SIZE;
	}
	
	// Make sure we are not overreaching ourselves.
	if (heap->start_address + sz <= heap->max_address)
		panic(NULL, "Heap tried to expand past max address 0x%X: 0x%X",
		      heap->max_address, heap->start_address + sz);
	
	// This should always be on a page boundary.
	uint32_t old_size = heap->end_address - heap->start_address;
	
	uint32_t i = old_size;
	while (i < sz)
	{
		AllocFrame(GetPage(heap->start_address + i, 1, kern_directory),
			     (heap->supervisor) ? true : false, (heap->readonly) ? true : false);
		i += PAGE_SIZE;
	}
	
	heap->end_address = heap->start_address + sz;
}

static uint32_t Contract(uint32_t sz, heap_t *heap)
{
	if (sz > heap->end_address - heap->start_address)
		panic(NULL, "Heap tried to contract below size 0x%X: 0x%X", sz,
		      heap->end_address - heap->start_address);
	
	// Get the nearest following page boundary.
	if (sz & PAGE_SIZE)
	{
		sz &= PAGE_SIZE;
		sz += PAGE_SIZE;
	}
	
	// Don't contract too far!
	if (sz < HEAP_MIN_SIZE)
		sz = HEAP_MIN_SIZE;
	
	uint32_t old_size = heap->end_address - heap->start_address;
	uint32_t i = old_size - PAGE_SIZE;
	while (sz < i)
	{
		FreeFrame(GetPage(heap->start_address + i, 0, kern_directory));
		i -= PAGE_SIZE;
	}
	
	heap->end_address = heap->start_address + sz;
	return sz;
}

static int32_t FindSmallestHole(uint32_t sz, uint8_t page_align, heap_t *heap)
{
	// Find the smallest hole that will fit.
	uint32_t i = 0;
	while (i < heap->index.size)
	{
		header_t *header = (header_t *)LookupOrderedArray(i, &heap->index);
		// If the user has requested the memory be page-aligned
		if (page_align > 0)
		{
			// Page-align the starting point of this header.
			uint32_t location = (uint32_t)header;
			int32_t offset = 0;
			
			if (((location + sizeof(header_t)) & 0xFFFFF000) != 0)
				// page size - (location + header) % alignment
				offset = PAGE_SIZE - (location + sizeof(header_t)) % PAGE_SIZE;
			
			int32_t hole_size = (int32_t)header->size - offset;
			
			// Can we fit now?
			if (hole_size >= (int32_t)sz)
				break;
		}
		else if (header->size >= sz)
			break;
		
		i++;
	}
	
	// Why did the loop exit?
	if (i == heap->index.size)
		return -1; // We got to the end and didn't find anything.
	else
		return i;
}

static int8_t HeaderPredicate(void *a, void *b)
{
	return (((header_t*)a)->size < ((header_t*)b)->size);
}

heap_t *CreateHeap(uint32_t start, uint32_t end_addr, uint32_t max, bool supervisor, bool readonly)
{
	heap_t *heap = kalloc(sizeof(heap_t));

	// All our assumptions are made on startAddress and endAddress being page-aligned.
	if (start % PAGE_SIZE)
		panic(NULL, "start address 0x%X is not page-aligned!", start);
	if (end_addr % PAGE_SIZE)
		panic(NULL, "end_addr 0x%X is not page-aligned!", end_addr);
	
	// Initialise the index.
	heap->index = PlaceOrderedArray((void*)start, HEAP_INDEX_SIZE, &HeaderPredicate);
	
	// Shift the start address forward to resemble where we can start putting data.
	start += sizeof(type_t) * HEAP_INDEX_SIZE;
	
	// Make sure the start address is page-aligned.
	if ((start & 0xFFFFF000) != 0)
	{
		start &= 0xFFFFF000;
		start += PAGE_SIZE;
	}
	
	// Write the start, end and max addresses into the heap structure.
	heap->start_address = start;
	heap->end_address   = end_addr;
	heap->max_address   = max;
	heap->supervisor    = supervisor;
	heap->readonly      = readonly;
	
	// We start off with one large hole in the index.
	header_t *hole      = (header_t *)start;
	hole->size          = end_addr - start;
	hole->magic         = HEAP_MAGIC;
	hole->is_hole       = true;
	hole->is_clean      = false;
	
	InsertOrderedArray((void*)hole, &heap->index);     
	
	return heap;
}

void *halloc(uint32_t size, uint8_t page_align, heap_t *heap)
{
	// Make sure we take the size of header/footer into account.
	uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
	// Find the smallest hole that will fit.
	int32_t i = FindSmallestHole(new_size, page_align, heap);
	
	if (i == -1) // If we didn't find a suitable hole
	{
		// Save some previous data.
		uint32_t old_length = heap->end_address - heap->start_address;
		uint32_t old_end_address = heap->end_address;
		
		// We need to allocate some more space.
		Expand(old_length+new_size, heap);
		uint32_t new_length = heap->end_address-heap->start_address;
		
		// Vars to hold the index of, and value of, the endmost header found so far.
		uint32_t idx = -1; uint32_t value = 0x0;
		// Find the endmost header. (Not endmost in size, but in location).
		for (i = 0; i < (int32_t)heap->index.size; i++)
		{
			uint32_t tmp = (uint32_t)LookupOrderedArray(i, &heap->index);
			if (tmp > value)
			{
				value = tmp;
				idx = i;
			}
		}
		
		// If we didn't find ANY headers, we need to add one.
		if (idx == (uint32_t)-1)
		{
			header_t *header = (header_t *)old_end_address;
			
			header->magic = HEAP_MAGIC;
			header->size = new_length - old_length;
			header->is_hole = true;
			header->is_clean = false;
			
			footer_t *footer = (footer_t *)(old_end_address + header->size - sizeof(footer_t));
			
			footer->magic = HEAP_MAGIC;
			footer->header = header;
			
			InsertOrderedArray((void*)header, &heap->index);
		}
		else
		{
			// The last header needs adjusting.
			header_t *header = LookupOrderedArray(idx, &heap->index);
			header->size += new_length - old_length;
			// Rewrite the footer.
			footer_t *footer = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
			footer->header = header;
			footer->magic = HEAP_MAGIC;
		}
		
		// We now have enough space. Recurse, and call the function again.
		return halloc(size, page_align, heap);
	}
	
	header_t *orig_hole_header = (header_t *)LookupOrderedArray(i, &heap->index);
	uint32_t orig_hole_pos     = (uint32_t)orig_hole_header;
	uint32_t orig_hole_size    = orig_hole_header->size;
	
	// Here we work out if we should split the hole we found into two parts.
	// Is the original hole size - requested hole size less than the overhead for adding a new hole?
	if (orig_hole_size-new_size < (sizeof(header_t) + sizeof(footer_t)))
	{
		// Then just increase the requested size to the size of the hole we found.
		size += orig_hole_size-new_size;
		new_size = orig_hole_size;
	}
	
	// If we need to page-align the data, do it now and make a new hole in front of our block.
	if (page_align && (orig_hole_pos & 0xFFFFF000))
	{
		uint32_t new_location = orig_hole_pos + PAGE_SIZE - (orig_hole_pos & 0xFFF) - sizeof(header_t);
		header_t *hole_header = (header_t *)orig_hole_pos;
		
		hole_header->size     = PAGE_SIZE - (orig_hole_pos & 0xFFF) - sizeof(header_t);
		hole_header->magic    = HEAP_MAGIC;
		hole_header->is_hole  = true;
		hole_header->is_clean      = false;
		
		footer_t *hole_footer = (footer_t *)((uint32_t)new_location - sizeof(footer_t));
		hole_footer->magic    = HEAP_MAGIC;
		hole_footer->header   = hole_header;
		
		orig_hole_pos         = new_location;
		orig_hole_size        = orig_hole_size - hole_header->size;
	}
	else // Else we don't need this hole any more, delete it from the index.
		RemoveOrderedArray(i, &heap->index);
	
	// Overwrite the original header...
	header_t *block_header  = (header_t *)orig_hole_pos;
	block_header->magic     = HEAP_MAGIC;
	block_header->is_hole   = false;
	block_header->is_clean  = false;
	block_header->size      = new_size;
	
	// ...And the footer
	footer_t *block_footer  = (footer_t *)(orig_hole_pos + sizeof(header_t) + size);
	block_footer->magic     = HEAP_MAGIC;
	block_footer->header    = block_header;
	
	// We may need to write a new hole after the allocated block.
	// We do this only if the new hole would have positive size...
	if (orig_hole_size - new_size > 0)
	{
		header_t *hole_header = (header_t *)(orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
		
		hole_header->magic    = HEAP_MAGIC;
		hole_header->is_hole  = true;
		hole_header->is_clean = false;
		hole_header->size     = orig_hole_size - new_size;
		
		footer_t *hole_footer = (footer_t *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(footer_t));
		if ((uint32_t)hole_footer < heap->end_address)
		{
			hole_footer->magic = HEAP_MAGIC;
			hole_footer->header = hole_header;
		}
		
		// Put the new hole in the index;
		InsertOrderedArray((void*)hole_header, &heap->index);
	}
	
	// ...And we're done!
	return (void *)((uint32_t)block_header + sizeof(header_t));
}

void hfree(void *p, heap_t *heap)
{
	// Exit gracefully for null pointers.
	if (p == NULL)
		return;
	
	// Get the header and footer associated with this pointer.
	header_t *header = (header_t*)((uint32_t)p - sizeof(header_t) );
	footer_t *footer = (footer_t*)((uint32_t)header + header->size - sizeof(footer_t) );
	
	// Sanity checks.
	if (header->magic != HEAP_MAGIC)
		panic(NULL, "Double free or corruption at address %p", p);
	
	if (footer->magic != HEAP_MAGIC)
		panic(NULL, "Double free or corruption at address %p", p);
	
	// Make us a hole.
	header->is_hole = true;
	header->is_clean = false;
	
	// Do we want to add this header into the 'free holes' index?
	bool do_add = true;
	
	// Unify left
	// If the thing immediately to the left of us is a footer...
	footer_t *test_footer = (footer_t*)((uint32_t)header - sizeof(footer_t));
	if (test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole)
	{
		uint32_t cache_size = header->size; // Cache our current size.
		header = test_footer->header;       // Rewrite our header with the new one.
		footer->header = header;            // Rewrite our footer to point to the new header.
		header->size += cache_size;         // Change the size.
		do_add = false;                     // Since this header is already in the index, we don't want to add it again.
	}
	
	// Unify right
	// If the thing immediately to the right of us is a header...
	header_t *test_header = (header_t*)((uint32_t)footer + sizeof(footer_t));
	if (test_header->magic == HEAP_MAGIC && test_header->is_hole)
	{
		// Increase our size.
		header->size += test_header->size; 
		
		// Rewrite it's footer to point to our header.
		test_footer = (footer_t*)((uint32_t)test_header + test_header->size - sizeof(footer_t));
		footer = test_footer;
		
		// Find and remove this header from the index.
		uint32_t i = 0;
		while ((i < heap->index.size) && (LookupOrderedArray(i, &heap->index) != (void*)test_header))
			i++;
		
		// Make sure we actually found the item.
		if (i > heap->index.size)
			panic (NULL, "Could not find pointer %p in heap!", p);
		
		// Remove it.
		RemoveOrderedArray(i, &heap->index);
	}
	
	// If the footer location is the end address, we can contract.
	if ((uint32_t)footer + sizeof(footer_t) == heap->end_address)
	{
		uint32_t old_length = heap->end_address-heap->start_address;
		uint32_t new_length = Contract((uint32_t)header - heap->start_address, heap);
		// Check how big we will be after resizing.
		if (header->size - (old_length-new_length) > 0)
		{
			// We will still exist, so resize us.
			header->size -= old_length-new_length;
			footer = (footer_t*)((uint32_t)header + header->size - sizeof(footer_t));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
		}
		else
		{
			// We will no longer exist :(. Remove us from the index.
			uint32_t i = 0;
			while ((i < heap->index.size) && (LookupOrderedArray(i, &heap->index) != (void*)test_header))
				i++;
			
			// If we didn't find ourselves, we have nothing to remove.
			if (i < heap->index.size)
				RemoveOrderedArray(i, &heap->index);
		}
	}
	
	// If required, add us to the index.
	if (do_add == true)
		InsertOrderedArray((void*)header, &heap->index);
	
}

void HeapCleanHoles(heap_t *heap)
{
	// Lookup all heap objects.
	for (uint32_t i = 0; i < heap->index.size; ++i)
	{
		header_t *head = LookupOrderedArray(i, &heap->index);
		
		// If we've found a hole that is not cleaned, clean it.
		if (head->is_hole && !head->is_clean)
		{
			// Wipe the memory block
			uint32_t *ptr = (void*)((uint32_t)head + sizeof(header_t));
			
			// While we don't hit the footer (or the end of the heap), clean the memory blocks.
			while (*ptr != HEAP_MAGIC && (uint32_t)head <= heap->end_address)
			{
				*ptr = 0;
				ptr++;
			}
			
			head->is_clean = true;
		}
	}
}
