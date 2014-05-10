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
#include "i386/pc/cpuid.h"

static inline void __cpuid(uint32_t i, uint32_t regs[4])
{
	__asm__ __volatile__
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
