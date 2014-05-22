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

// Attribute definitions
// see https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
#define __noreturn         __attribute__((noreturn))
#define __noinline         __attribute__((noinline))
#define __naked            __attribute__((naked))
#define __packed           __attribute__((packed))
#define __nonull(args...)  __attribute__((nonnull(args...)))
#define __neverretnull     __attribute__((returns_nonnull))
#define __flatten          __attribute__((flatten))
#define __inline_damnit    __attribute__((always_inline))
#define __pure             __attribute__((pure))
#define __hot              __attribute__((hot))
#define __cold             __attribute__((cold))
#define __section(x)       __attribute__((section(x)))
#define __cdecl            __attribute__((cdecl))
#define __stdcall          __attribute__((stdcall))
#define __fastcall         __attribute__((fastcall))
#define __thiscall         __attribute__((thiscall))
#define __unused           __attribute__((unused))
#define __used             __attribute__((used))
#define __vishidden        __attribute__((visibility("hidden")))
#define __visprotected     __attribute__((visibility("protected")))
#define __visinternal      __attribute__((visibility("internal")))
#define __visdefault       __attribute__((visibility("default")))
#define __warn_unused_res  __attribute__((warn_unused_result))
#define __section(x)       __attribute__((section(x)))
#define __deprecated(x)    __attribute__((deprecated(x)))
#define __warning(x)       __attribute__((warning(x)))
#define __error(x)         __attribute__((error(x)))
#define __malloc           __attribute__((malloc))

// -- I/O Bus communication functions.
extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

// -- Sleep functions
extern void sleep(uint32_t secs);

// -- Critical Section definitions
// First disables all maskable interrupts
// the second re-enables them
static inline void EnterCriticalSection(void) { __asm__ __volatile__("cli"); }
static inline void ExitCriticalSection(void)  { __asm__ __volatile__("sti"); }

// -- Registers struct.
typedef struct registers
{
	uint32_t ds;                  // Data segment selector
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	uint32_t int_no, is_irq, err_code;    // Interrupt number,  error code (if applicable)
	uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

#endif // __KCOMMON_H__
