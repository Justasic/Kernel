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
#include "i386/pc/syscall.h"

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

void ISRHandler(registers_t regs)
{
	switch (regs.int_no)
	{
		case DIVISION_BY_ZERO:
			panic(&regs, "Kernel attempted to end the world with a division by zero which was saved by the CPU.");
			break;
		case DEBUG_EXCEPTION:
			prnt(DEBUG_EXCEPTION);
			break;
		case NON_MASKABLE_INTERRUPT:
			panic(&regs, "Non-maskable interrupt caught by CPU.");
			break;
		case BREAKPOINT_EXCEPTION:
			prnt(BREAKPOINT_EXCEPTION);
			break;
		case INTRO_DETECTION:
			prnt(INTRO_DETECTION);
			break;
		case OUT_OF_BOUNDS_EXCEPTION:
			panic(&regs, "The kernel tried to leave the playground and the CPU said \"NO! >:(\" (Out-of-bounds Exception)");
			break;
		case INVALID_OPCODE:
			panic(&regs, "Invalid Opcode exception caught by CPU.");
			break;
		case NO_COPROCESSOR:
			prnt(NO_COPROCESSOR);
			break;
		case DOUBLE_FAULT:
			panic(&regs, "Double fault caught by CPU.");
			break;
		case COPROCESSOR_SEGMENT_OVERRUN:
			panic(&regs, "Co-Processor segment overrun caught by CPU.");
			break;
		case BAD_TSS:
			prnt(BAD_TSS);
			break;
		case SEGMENT_NOT_PRESENT:
			prnt(SEGMENT_NOT_PRESENT);
			break;
		case STACK_FAULT:
			panic(&regs, "Stack Fault caught by CPU.");
			break;
		case GENERAL_PROTECTION:
			panic(&regs, "General Protection Fault caught by CPU.");
			break;
		case PAGE_FAULT:
		{
			EnterCriticalSection();
			// The faulting address is stored in the CR2 register.
			uint32_t faulting_address;
			__asm__ __volatile__("mov %%cr2, %0" : "=r" (faulting_address));
			
			// The error code gives us details of what happened.
			int present   = !(regs.err_code & 0x1); // Page not present
			int rw = regs.err_code & 0x2;           // Write operation?
			int um = regs.err_code & 0x4;           // Processor was in user-mode?
			int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
			int id = regs.err_code & 0x10;          // Caused by an instruction fetch?
			panic(&regs, "Page fault (%s %s %s %s) at 0x%X",
			      um ? "Usermode," : "",
			      rw ? "Read-Only," : "",
			      present ? "Page not present," : "",
			      reserved ? "Reserved" : "", faulting_address);
			ExitCriticalSection();
			break;
		}
		case UNKNOWN_INTERRUPT:
			panic(&regs, "Unknown interrupt passed to CPU.");
			break;
		case COPROCESSOR_FAULT:
			prnt(COPROCESSOR_FAULT);
			break;
		case ALIGNMENT_CHECK:
			prnt(ALIGNMENT_CHECK);
			break;
		case MACHINE_CHECK:
			panic(&regs, "Machine Check Exception caught by CPU.");
			break;
		case 0x80:
			printf("Received syscall interrupt with syscall 0x%X\n", regs.eax);
			HandleSyscall(regs);
			break;
		default:
			prnt(UNKNOWN_INTERRUPT);
			printf("Unknown interrupt: %d\n", regs.int_no);
	}
}
