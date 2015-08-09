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


#define PORT_DMA_ADDR_2        0x0004
#define PORT_DMA_CNT_2         0x0005
#define PORT_DMA1_MASK_REG     0x000a
#define PORT_DMA1_MODE_REG     0x000b
#define PORT_DMA1_CLEAR_FF_REG 0x000c
#define PORT_DMA1_MASTER_CLEAR 0x000d
#define PORT_PIC1_CMD          0x0020
#define PORT_PIC1_DATA         0x0021
#define PORT_PIT_COUNTER0      0x0040
#define PORT_PIT_COUNTER1      0x0041
#define PORT_PIT_COUNTER2      0x0042
#define PORT_PIT_MODE          0x0043
#define PORT_PS2_DATA          0x0060
#define PORT_PS2_CTRLB         0x0061
#define PORT_PS2_STATUS        0x0064
#define PORT_CMOS_INDEX        0x0070
#define PORT_CMOS_DATA         0x0071
#define PORT_DIAG              0x0080
#define PORT_DMA_PAGE_2        0x0081
#define PORT_A20               0x0092
#define PORT_PIC2_CMD          0x00a0
#define PORT_PIC2_DATA         0x00a1
#define PORT_SMI_CMD           0x00b2
#define PORT_SMI_STATUS        0x00b3
#define PORT_DMA2_MASK_REG     0x00d4
#define PORT_DMA2_MODE_REG     0x00d6
#define PORT_DMA2_MASTER_CLEAR 0x00da
#define PORT_MATH_CLEAR        0x00f0
#define PORT_ATA2_CMD_BASE     0x0170
#define PORT_ATA1_CMD_BASE     0x01f0
#define PORT_LPT2              0x0278
#define PORT_SERIAL4           0x02e8
#define PORT_SERIAL2           0x02f8
#define PORT_ATA2_CTRL_BASE    0x0374
#define PORT_LPT1              0x0378
#define PORT_SERIAL3           0x03e8
#define PORT_ATA1_CTRL_BASE    0x03f4
#define PORT_FD_BASE           0x03f0
#define PORT_FD_DOR            0x03f2
#define PORT_FD_STATUS         0x03f4
#define PORT_FD_DATA           0x03f5
#define PORT_HD_DATA           0x03f6
#define PORT_FD_DIR            0x03f7
#define PORT_SERIAL1           0x03f8
#define PORT_PCI_CMD           0x0cf8
#define PORT_PCI_REBOOT        0x0cf9
#define PORT_PCI_DATA          0x0cfc
#define PORT_BIOS_DEBUG        0x0402
#define PORT_QEMU_CFG_CTL      0x0510
#define PORT_QEMU_CFG_DATA     0x0511
#define PORT_ACPI_PM_BASE      0xb000
#define PORT_SMB_BASE          0xb100
#define PORT_BIOS_APM          0x8900


// -- I/O Bus communication functions.
extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern void outw(uint16_t port, uint16_t value);
extern uint32_t inl(uint16_t port);
extern void outl(uint16_t port, uint32_t value);

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
