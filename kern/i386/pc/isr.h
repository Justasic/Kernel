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

#ifndef __KISR_H__
#define __KISR_H__

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

enum
{
	IRQ0 = 32,
	IRQ1,
	IRQ2,
	IRQ3,
	IRQ4,
	IRQ5,
	IRQ6,
	IRQ7,
	IRQ8,
	IRQ9,
	IRQ10,
	IRQ11,
	IRQ12,
	IRQ13,
	IRQ14,
	IRQ15,
};

typedef void (*isr_t)(registers_t);
extern void RegisterIRQHandler(uint8_t n, isr_t handler);
extern void ISRHandler(registers_t regs);

#endif // __KISR_H__
