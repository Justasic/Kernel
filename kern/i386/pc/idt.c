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
#include <string.h>
#include <stdio.h>
#include "lib/common.h"

// A struct describing an interrupt gate.
struct idt_entry_struct
{
	uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel;                 // Kernel segment selector.
	uint8_t  always0;             // This must always be zero.
	uint8_t  flags;               // More flags. See documentation.
	uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __packed;
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
	uint16_t limit;
	uint32_t base;                // The address of the first element in our idt_entry_t array.
} __packed;
typedef struct idt_ptr_struct idt_ptr_t;

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

// FlushIDT function -- defined in gdt.S
extern void FlushIDT(uint32_t);

// All of the interrupts.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();
extern void isr19 ();
extern void isr20 ();
extern void isr21 ();
extern void isr22 ();
extern void isr23 ();
extern void isr24 ();
extern void isr25 ();
extern void isr26 ();
extern void isr27 ();
extern void isr28 ();
extern void isr29 ();
extern void isr30 ();
extern void isr31 ();

// Syscall interrupt
extern void isr128();

// IRQ interrupts
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10 ();
extern void irq11 ();
extern void irq12 ();
extern void irq13 ();
extern void irq14 ();
extern void irq15 ();

static void IDTSetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
	
	idt_entries[num].sel     = sel;
	idt_entries[num].always0 = 0;
	// We must uncomment the OR below when we get to using user-mode.
	// It sets the interrupt gate's privilege level to 3.
	idt_entries[num].flags   = flags /* | 0x60 */;
} 

#define SetGate(x) IDTSetGate(x, (uint32_t)isr##x , 0x08, 0x8E);

void InitializeIDT(void)
{
	idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
	idt_ptr.base  = (uint32_t)&idt_entries;
	
	memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);
	
	// Remap the irq table.
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	
	// We could make this a loop but unrolled loops are awesome :3
	SetGate(0);
	SetGate(1);
	SetGate(2);
	SetGate(3);
	SetGate(4);
	SetGate(5);
	SetGate(6);
	SetGate(7);
	SetGate(8);
	SetGate(9);
	SetGate(10);
	SetGate(11);
	SetGate(12);
	SetGate(13);
	SetGate(14);
	SetGate(15);
	SetGate(16);
	SetGate(17);
	SetGate(18);
	SetGate(19);
	SetGate(20);
	SetGate(21);
	SetGate(22);
	SetGate(23);
	SetGate(24);
	SetGate(25);
	SetGate(26);
	SetGate(27);
	SetGate(28);
	SetGate(29);
	SetGate(30);
	SetGate(31);
	
	SetGate(128); // 0x80
	
	// Unrolled IRQ loop to initialize the IRQs
	IDTSetGate(32, (uint32_t)irq0, 0x08, 0x8E);
	IDTSetGate(33, (uint32_t)irq1, 0x08, 0x8E);
	IDTSetGate(34, (uint32_t)irq2, 0x08, 0x8E);
	IDTSetGate(35, (uint32_t)irq3, 0x08, 0x8E);
	IDTSetGate(36, (uint32_t)irq4, 0x08, 0x8E);
	IDTSetGate(37, (uint32_t)irq5, 0x08, 0x8E);
	IDTSetGate(38, (uint32_t)irq6, 0x08, 0x8E);
	IDTSetGate(39, (uint32_t)irq7, 0x08, 0x8E);
	IDTSetGate(40, (uint32_t)irq8, 0x08, 0x8E);
	IDTSetGate(41, (uint32_t)irq9, 0x08, 0x8E);
	IDTSetGate(42, (uint32_t)irq10, 0x08, 0x8E);
	IDTSetGate(43, (uint32_t)irq11, 0x08, 0x8E);
	IDTSetGate(44, (uint32_t)irq12, 0x08, 0x8E);
	IDTSetGate(45, (uint32_t)irq13, 0x08, 0x8E);
	IDTSetGate(46, (uint32_t)irq14, 0x08, 0x8E);
	IDTSetGate(47, (uint32_t)irq15, 0x08, 0x8E);
	
	printf("Flushing interrupts\n");
	
	FlushIDT((uint32_t)&idt_ptr);
	
	printf("Flush complete\n");
}
