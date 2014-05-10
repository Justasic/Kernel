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
#include <stdio.h>
#include "lib/common.h"
#include "lib/panic.h"

enum 
{
	DIVISION_BY_ZERO,          // Division by zero exception
	DEBUG_EXCEPTION,           // Debug exception
	NON_MASKABLE_INTERRUPT,    // Non-maskable interrupt
	BREAKPOINT_EXCEPTION,      // Breakpoint exception
	INTRO_DETECTION,           // 'Intro detected overflow'
	OUT_OF_BOUNDS_EXCEPTION,   // Out of bounds exception
	INVALID_OPCODE,            // Invalid opcode exception
	NO_COPROCESSOR,            // No coprocessor exception
	DOUBLE_FAULT,              // Double fault (w/ error code)
	COPROCESSOR_SEGMENT_OVERRUN,// Coprocessor segment overrun
	BAD_TSS,                   // Bad TSS (w/ error code)
	SEGMENT_NOT_PRESENT,       // Segment not present (w/ error code)
	STACK_FAULT,               // Stack Fault (w/ error code)
	GENERAL_PROTECTION,        // General protection fault (w/ error code)
	PAGE_FAULT,                // Page fault (w/ error code)
	UNKNOWN_INTERRUPT,         // Unknown interrupt exception
	COPROCESSOR_FAULT,         // Coprocessor fault
	ALIGNMENT_CHECK,           // Alignment check exception
	MACHINE_CHECK              // Machine check exception
};

#define prnt(x) printf(#x " happened\n");

void isr_handler(registers_t regs)
{
	switch (regs.int_no)
	{
		case DIVISION_BY_ZERO:
			prnt(DIVISION_BY_ZERO);
			panic("Kernel attempted to end the world with a division by zero which was saved by the CPU.", &regs);
			break;
		case DEBUG_EXCEPTION:
			prnt(DEBUG_EXCEPTION);
			break;
		case NON_MASKABLE_INTERRUPT:
			prnt(NON_MASKABLE_INTERRUPT);
			panic("Non-maskable interrupt caught by CPU.", &regs);
			break;
		case BREAKPOINT_EXCEPTION:
			prnt(BREAKPOINT_EXCEPTION);
			break;
		case INTRO_DETECTION:
			prnt(INTRO_DETECTION);
			break;
		case OUT_OF_BOUNDS_EXCEPTION:
			prnt(OUT_OF_BOUNDS_EXCEPTION);
			panic("The kernel tried to leave the playground and the CPU said \"NO! >:(\" (Out-of-bounds Exception)", &regs);
			break;
		case INVALID_OPCODE:
			prnt(INVALID_OPCODE);
			panic("Invalid Opcode exception caught by CPU.", &regs);
			break;
		case NO_COPROCESSOR:
			prnt(NO_COPROCESSOR);
			break;
		case DOUBLE_FAULT:
			prnt(DOUBLE_FAULT);
// 			panic("Double fault caught by CPU.", &regs);
			break;
		case COPROCESSOR_SEGMENT_OVERRUN:
			prnt(COPROCESSOR_SEGMENT_OVERRUN);
			panic("Co-Processor segment overrun caught by CPU.", &regs);
			break;
		case BAD_TSS:
			prnt(BAD_TSS);
			break;
		case SEGMENT_NOT_PRESENT:
			prnt(SEGMENT_NOT_PRESENT);
			break;
		case STACK_FAULT:
			prnt(STACK_FAULT);
			panic("Stack Fault caught by CPU.", &regs);
			break;
		case GENERAL_PROTECTION:
			prnt(GENERAL_PROTECTION);
// 			panic("General Protection Fault caught by CPU.", &regs);
			break;
		case PAGE_FAULT:
			prnt(PAGE_FAULT);
			panic("Page fault caught by CPU.", &regs);
			break;
		case UNKNOWN_INTERRUPT:
			prnt(UNKNOWN_INTERRUPT);
			panic("Unknown interrupt passed to CPU.", &regs);
			break;
		case COPROCESSOR_FAULT:
			prnt(COPROCESSOR_FAULT);
			break;
		case ALIGNMENT_CHECK:
			prnt(ALIGNMENT_CHECK);
			break;
		case MACHINE_CHECK:
			prnt(MACHINE_CHECK);
			panic("Machine Check Exception caught by CPU.", &regs);
			break;
		case 0x80:
			printf("Received syscall interrupt\n");
			break;
		default:
			prnt(UNKNOWN_INTERRUPT);
			printf("Unknown interrupt: %d\n", regs.int_no);
	}
}
