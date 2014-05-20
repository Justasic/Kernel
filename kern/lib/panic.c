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
#include "lib/common.h"
#include "mm/paging.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <tty/vga_terminal.h>

// Color to differ the message from the other text.
const vga_color_t color = vga_color(COLOR_BLACK, COLOR_WHITE);

// static registers_t regs;
void PrintStackTrace(unsigned int MaxFrames)
{
	// Get our ebp value before disabling paging, this will screw up the
	// stack trace if we don't
	unsigned int *ebp = &MaxFrames - 2;
	
	printcf(color, "Stack Trace:\n");
	
	for (unsigned int frame = 0; frame < MaxFrames; ++frame)
	{
		// Read http://fabiensanglard.net/macosxassembly/index.php
		// to understand this more
		unsigned int eip = ebp[1]; // get old EIP
		if (eip == 0)
			// No caller on stack, we've gone back as far as we can
			break;
		ebp = (unsigned int *)ebp[0];
		unsigned int *args = &ebp[2];
		printcf(color, " Frame[%d]: 0x%X\n", frame, eip);
	}
}

// Naked function so we can dump registers without
// the compiler modifying them before we can dump.
___ATTRIB_FORMAT__(2, 3)
void panic(registers_t *regs, char *err, ...)
{
	// Disable interrupts and paging so we don't cause issues
	// we're going to be halting soon anyway so none of this matters
	EnterCriticalSection();
	
	// If we have registers, go ahead and dump the CR0 and CR3 registers.
	// This !!MUST!! be done before we disable paging or the addresses will
	// be invalidated.
	uint32_t cr0 = 0, cr3 = 0;
	if (regs)
	{
		__asm__ __volatile__("movl %%cr0, %0\n\t"
				     "movl %%cr3, %1\n\t"
				     : "=r" (cr0), "=r" (cr3)
		);
	}
	
	// NOTE: It's probably bad that we're disabling paging before
	// we start printing a stack trace. There should be a better
	// way to walk the stack and print messages than having to
	// disable paging.
	DisablePaging();
	
	// Convert the arguments to a nice formatted message
	va_list ap;
	char buf[(1 << 7)];
	va_start(ap, err);
	vsnprintf(buf, sizeof(buf), err, ap);
	va_end(ap);
	
	// Now that we're safe from register clobbering from function calls, we can
	// call the print functions to dump those registers to the terminal.
	printcf(vga_color(COLOR_BLACK, COLOR_LIGHT_RED), "PANIC! %s\n", buf);
	
	// If we have registers to print (which we likely will) then dump them
	if (regs)
	{

		// Print the registers
		printcf(color, "\nRegisters:\n");
		printcf(color, " eax: ................. 0x%-10X ebx: ................. 0x%X\n ecx: ................. 0x%-10X", regs->eax, regs->ebx, regs->ecx);
		printcf(color, " edx: ................. 0x%X\n esp: ................. 0x%-10X ebp: ................. 0x%X\n", regs->edx, regs->esp, regs->ebp);
		printcf(color, " eip: ................. 0x%-10X cr0: ................. 0x%X\n cr3: ................. 0x%X\n", regs->eip, cr0, cr3);
		printcf(color, " Interrupt Number: .... 0x%X\n", regs->int_no);
		printcf(color, " Interrupt Type: ...... 0x%X\n", regs->is_irq);
		printcf(color, " Error Code: .......... 0x%X\n", regs->err_code);
		printcf(color, " EFLAGS: .............. 0x%X\n", regs->eflags);
		printcf(color, " User ESP: ............ 0x%X\n\n", regs->useresp);
	}
	
	// Print a stack trace so we know wtf is going on in the stack
	PrintStackTrace(5);
	
	// We're done with whatever we're doing.
	// Hang the PC until the user forces a reboot.
	__asm__ __volatile__(
		".hang:\n\t"    // Make a label
		"cli\n\t"       // Clear all non-maskable interrupts
		"hlt\n\t"       // Halt the kernel
		"jmp .hang\n\t" // Jump to our label above if an interrupt happens
	);
}
