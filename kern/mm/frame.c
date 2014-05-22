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
#include "lib/panic.h"

// A bitset of frames - used or free.
uint32_t *frames;
uint32_t nframes;

// Defined in kheap.c
extern uint32_t placement_address;

// Static function to set a bit in the frames bitset
static void SetFrame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void ClearFrame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t TestFrame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t FirstFrame(void)
{
	uint32_t i, j;
	for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
	{
		if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
		{
			// at least one bit is free here.
			for (j = 0; j < 32; j++)
			{
				uint32_t toTest = 0x1 << j;
				if (!(frames[i] & toTest))
					return i * 4 * 8 + j;
			}
		}
	}
	return 0;
}

// Function to allocate a frame.
void AllocFrame(page_t *page, bool is_kernel, bool is_writeable)
{
	if (page->frame != 0)
		return; // Frame was already allocated, return straight away.
	else
	{
		uint32_t idx = FirstFrame(); // idx is now the index of the first free frame.
		if (idx == (uint32_t)-1)
			panic(NULL, "No free frames to allocate!");
		
		SetFrame(idx * 0x1000); // this frame is now ours!
		page->present = 1; // Mark it as present.
		page->rw = (is_writeable) ? 1 : 0; // Should the page be writeable?
		page->user = (is_kernel) ? 0 : 1; // Should the page be user-mode?
		page->frame = idx;
	}
}

// Function to deallocate a frame.
void FreeFrame(page_t *page)
{
	uint32_t frame;
	if (!(frame = page->frame))
		return; // The given page didn't actually have an allocated frame!
		
	ClearFrame(frame); // Frame is now free again.
	page->frame = 0x0; // Page now doesn't have a frame.
} 
