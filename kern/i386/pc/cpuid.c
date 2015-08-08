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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/common.h"
#include "i386/pc/cpuid.h"

static inline void __cpuid(uint32_t i, uint32_t regs[4])
{
	__asm__
	// opcode    EAX             EBX              ECX            EDX
	("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
	// EAX      ECX -- Inputs
	: "a" (i), "c" (0));
	// ECX is set to zero for CPUID function 4
}

// Get the CPUID model
char *GetCPUVendor(void)
{
	static char str[13];
	static uint8_t ran = 0;
	if (!ran)
	{
		uint32_t regs[4];
		
		__cpuid(CPUID_GETVENDORSTRING, regs);
		
		((unsigned *)str)[0] = regs[1]; // EBX
		((unsigned *)str)[1] = regs[3]; // EDX
		((unsigned *)str)[2] = regs[2]; // ECX
		str[12] = 0;
		ran = 1;
	}
	return str;
}

uint8_t CPUSupportsFeature(uint32_t featureid)
{
	uint32_t regs[4];
	__cpuid(CPUID_GETFEATURES, regs);
	
	// Get ECX register
	return (regs[2] & featureid);
}

uint8_t CPUSupportsFeatureExt(uint32_t featureid)
{
	uint32_t regs[4];
	__cpuid(CPUID_GETFEATURES, regs);
	
	// Get EDX register
	return (regs[3] & featureid);
}

#include <stdio.h>
bool CPUSupportsCPUID(void)
{
#if 1
	
	__asm__("pushfd\n\t");
	printf("Flags: 0x%X\n");
	// here this dumps the flags register, compares it
	// then returns it.
	uint8_t i = 0;
	uint8_t save = 0;
	// Get the EFLAGS register into our integer.
	__asm__ ("pushfd\n\t"
		"pop %%eax\n\t"
		:: "a" (i)
	);
	
	save = i;
	i |= (1 << 21);
	
	// Add it back then get it again
	__asm__ ("pushl %%eax\n\t"
		"popfd\n\t"
		"pushfd\n\t"
		"popl %%eax\n\t"
		: "=a" (i)
		: "0" (i)
	);
	
	printf("eflags: 0x%X\nSave: 0x%X\n", i, save);
	
	i ^= save;
	printf("Blah: %b\n", i);
	return (i & (1 << 21)) != 0;
	
#else
	uint8_t i = 0;
	__asm__ (
		"pushfd\n\t"               // Push the flags to the stack
		"pop %%eax\n\t"            // Move the eflags to eax. This is a dumb way to modify eflags but whatever.
		"movl %%eax, %%ecx\n\t"    // save for later
		"xor $0x200000, %%eax\n\t" // bitflip
		"push %%eax\n\t"           // Push back to the stack
		"popfd\n\t"                // Pop flags register to set it.
		"pushfd\n\t"               // Get the flags register
		"pop %%eax\n\t"            // get eax
		"xor %%ecx, %%eax\n\t"     // diff them and see what changed
		"shr $21, %%eax\n\t"       // move bit 21 to bit 0
		"and $0x1, %%eax\n\t"      // dump the other bits
		"popfd\n\t"                // restore original flags
		:: "a" (i)
	);
	
	printf("b;ah: %b\n", i);
	return i;
#endif
}

void GetCPUInfo(void)
{
// 	if (!CPUSupportsCPUID())
// 	{
// 		printf("CPU does not support CPUID instruction.\n");
// 		return;
// 	}
	
	printf("CPU: %s\n", GetCPUVendor());
	printf("CPU has FPU: %b\n", CPUSupportsFeature(CPUID_FEAT_EDX_FPU));
}
