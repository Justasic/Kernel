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

#include "panic.h"
#include "common.h"
#include <stdio.h>
#include <stdint.h>

// Naked function so we can dump registers without
// the compiler modifying them before we can dump.
void __attribute__((naked)) panic(char *err)
{
	// There's probably a way to use pusha, pushf, and some
	// other opcodes to push all these to the stack and then
	// use the registers_t struct
#if 0
	
	// Save the registers to a struct.
	__asm__ __volatile__(
		"movl %%eax, %0\n\t"
		"movl %%ebx, %1\n\t"
		"movl %%ecx, %2\n\t"
		"movl %%edx, %3\n\t"
		"movl %%esp, %4\n\t"
		"movl %%ebp, %5\n\t"
		"movl %%esi, %6\n\t"
		"movl %%edi, %7\n\t"
		"movl %%eip, %8\n\t"
		/* output operands   */
		: "=r" (regs.eax), "=r" (regs.ebx), "=r" (regs.ecx), "=r" (regs.edx),
		     "=r" (regs.esp), "=r" (regs.ebp), "=r" (regs.esi), "=r" (regs.edi),
			"=r" (regs.eip)
		: /* input operands    */
		: /* clobber registers */
	);
#endif
	
	// Now that we're safe from register clobbering from function calls, we can
	// call the print functions to dump those registers to the terminal.
	printcf(vga_color(COLOR_BLACK, COLOR_LIGHT_RED), "PANIC! %s\n", err);
	
#if 0
	// Print the registers
	printf("\nRegisters:\n");
	printf(" eax: ................. %x\n ebx: ................. %x\n ecx: ................. %x\n", regs.eax, regs.ebx, regs.ecx);
	printf(" edx: ................. %x\n esp: ................. %x\n ebp: ................. %x\n", regs.edx, regs.esp, regs.ebp);
	printf(" eip: ................. %x\n cr0: ................. %x\n cr3: ................. %x\n", regs.eip, cr0, cr3);
	printf(" Interrupt Number: .... %x\n", regs.int_no);
	printf(" Error Code: .......... %x\n", regs.err_code);
	printf(" EFLAGS: .............. %x\n", regs.eflags);
	printf(" User ESP: ............ %x\n", regs.useresp);
#endif
	
	// Print a stack trace so we know wtf is going on
	printf("\n\nStack Trace:\n");
	printf(" Unimplemented\n");
	
	// We're done with whatever we're doing.
	// Hang the PC until the user forces a reboot.
	__asm__ __volatile__(
		".hang:\n\t"    // Make a label
		"cli\n\t"       // Clear all non-maskable interrupts
		"hlt\n\t"       // Halt the kernel
		"jmp .hang\n\t" // Jump to our label above if an interrupt happens
	);
}
