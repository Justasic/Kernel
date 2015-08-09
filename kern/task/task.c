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
#include "task.h"
#include <stdint.h>
#include <string.h>
#include "mm/paging.h"
#include "mm/frame.h"

extern uint32_t start_stack;

void move_stack(void *new_stack_start, uint32_t size)
{
	uint32_t i;

	for (i = (uint32_t)new_stack_start; i >= ((uint32_t)new_stack_start - size); i -= 0x1000)
		AllocFrame(GetPage(i, 1, cur_directory), 0, 1);

	// Flush the TLB
	uint32_t pd_addr;
	__asm__ __volatile__("mov %%cr3, %0" : "=r"(pd_addr));
	__asm__ __volatile__("mov %0, %%cr3" :: "r"(pd_addr));

	// old ESP and EBP
	uint32_t old_stack_pointer;		__asm__ __volatile__("mov %%esp, %0" : "=r"(old_stack_pointer));
	uint32_t old_base_pointer;		__asm__ __volatile__("mov %%ebp, %0" : "=r"(old_base_pointer));

	uint32_t offset			     = (uint32_t)new_stack_start - start_stack;
	uint32_t new_stack_pointer	 = old_stack_pointer + offset;
	uint32_t new_base_pointer    = old_base_pointer + offset;

	// Copy the stack
	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, start_stack - old_stack_pointer);

	for (i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start - size; i -= 4)
	{
		uint32_t tmp = *(uint32_t*)i;

		if ((old_stack_pointer < tmp) && (tmp < start_stack))
		{
			tmp = tmp + offset;
			uint32_t *tmp2 = (uint32_t*)i;
			*tmp2 = tmp;
		}
	}

	// swap stacks
	__asm__ __volatile__("mov %0, %%esp" :: "r"(new_stack_pointer));
	__asm__ __volatile__("mov %0, %%ebp" :: "r"(new_base_pointer));
}

