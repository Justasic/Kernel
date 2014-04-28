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
#ifndef __KCOMMON_H__
#define __KCOMMON_H__
#include <stdint.h>

// -- I/O Bus communication functions.
extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

// -- Critical Section definitions
// First disables all maskable interrupts
// the second re-enables them
inline void EnterCriticalSection(void) { __asm__ __volatile__("cli"); }
inline void ExitCriticalSection(void)  { __asm__ __volatile__("sti"); }

// -- Registers struct.
typedef struct registers
{
	uint32_t ds;                  // Data segment selector
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t; 

#endif // __KCOMMON_H__
