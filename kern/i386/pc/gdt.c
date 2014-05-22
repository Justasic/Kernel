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
#include <stdint.h>
#include <string.h>
#include "lib/common.h"

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry_s
{
	unsigned int limit_low:16;
	unsigned int base_low : 24;
     //attribute byte split into bitfields
	unsigned int accessed :1;
	unsigned int read_write :1; //readable for code, writable for data
	unsigned int conforming_expand_down :1; //conforming for code, expand down for data
	unsigned int code :1; //1 for code, 0 for data
	unsigned int always_1 :1; //should be 1 for everything but TSS and LDT
	unsigned int DPL :2; //priveledge level
	unsigned int present :1;
     //and now into granularity
	unsigned int limit_high :4;
	unsigned int available :1;
	unsigned int always_0 :1; //should always be 0
	unsigned int big :1; //32bit opcodes for code, dword stack for data
	unsigned int gran :1; //1 to use 4k page addressing, 0 for byte addressing
	unsigned int base_high :8;
} __packed;

struct gdt_ptr_s
{
	uint16_t limit;               // The upper 16 bits of all selector limits.
	uint32_t base;                // The address of the first gdt_entry_t struct.
} __packed;

typedef struct gdt_ptr_s gdt_ptr_t; 
typedef struct gdt_entry_s gdt_entry_t;

// Extern for our gdt_flush function
// this is defined in gdt.S
extern void FlushGDT(uint32_t);

// Global variables to the Global Descriptor Table
gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

void InitializeGDT(void)
{
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base  = (uint32_t)&gdt_entries;
	
	// Null segment
	memset(&gdt_entries[0], 0, sizeof(gdt_entry_t));
	
	// Code segment
	gdt_entries[1].limit_low = 0x0000FFFF; // 65535
	gdt_entries[1].base_low = 0;
	gdt_entries[1].accessed = 0;
	gdt_entries[1].read_write = 1;
	gdt_entries[1].conforming_expand_down = 0;
	gdt_entries[1].code = 1;
	gdt_entries[1].always_1 = 1;
	gdt_entries[1].DPL = 0;
	gdt_entries[1].present = 1;
	gdt_entries[1].limit_high = 15;
	gdt_entries[1].available = 0;
	gdt_entries[1].always_0 = 0;
	gdt_entries[1].big = 1;
	gdt_entries[1].gran = 1;
	gdt_entries[1].base_high = 0;
	
	// Data segment
	gdt_entries[2].limit_low = 0x0000FFFF; // 65535
	gdt_entries[2].base_low = 0;
	gdt_entries[2].accessed = 0;
	gdt_entries[2].read_write = 1;
	gdt_entries[2].conforming_expand_down = 0;
	gdt_entries[2].code = 0; 
	gdt_entries[2].always_1 = 1;
	gdt_entries[2].DPL = 0; 
	gdt_entries[2].present = 1;
	gdt_entries[2].limit_high = 15;
	gdt_entries[2].available = 0;
	gdt_entries[2].always_0 = 0;
	gdt_entries[2].big = 1;
	gdt_entries[2].gran = 1;
	gdt_entries[2].base_high = 0;
	
	// User mode code segment
	gdt_entries[3].limit_low = 0x0000FFFF; // 65535
	gdt_entries[3].base_low = 0;
	gdt_entries[3].accessed = 0; 
	gdt_entries[3].read_write = 1;
	gdt_entries[3].conforming_expand_down = 0;
	gdt_entries[3].code = 1;
	gdt_entries[3].always_1 = 1;
	gdt_entries[3].DPL = 3;
	gdt_entries[3].present = 1;
	gdt_entries[3].limit_high = 15;
	gdt_entries[3].available = 0;
	gdt_entries[3].always_0 = 0;
	gdt_entries[3].big = 1;
	gdt_entries[3].gran = 1;
	gdt_entries[3].base_high = 0;
	
	// User mode data segment
	gdt_entries[4].limit_low = 0x0000FFFF; // 65535
	gdt_entries[4].base_low = 0;
	gdt_entries[4].accessed = 0;
	gdt_entries[4].read_write = 1;
	gdt_entries[4].conforming_expand_down = 0;
	gdt_entries[4].code = 0;
	gdt_entries[4].always_1 = 1;
	gdt_entries[4].DPL = 3;
	gdt_entries[4].present = 1;
	gdt_entries[4].limit_high = 15;
	gdt_entries[4].available = 0;
	gdt_entries[4].always_0 = 0;
	gdt_entries[4].big = 1;
	gdt_entries[4].gran = 1;
	gdt_entries[4].base_high = 0;
	
	FlushGDT((uint32_t)&gdt_ptr);
}
