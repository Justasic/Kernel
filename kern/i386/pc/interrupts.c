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
#include <lib/common.h>
#include <stdlib.h>
#include <stdio.h>

extern void init_gdt(void);
extern void init_idt(void);
void initialize_descriptor_tables(void)
{
	printf("Installing Global Descriptor Table\n");
	init_gdt();
	printf("Installing Interrupt Descriptor Table\n");
	init_idt();
}

extern void irq_handler(registers_t regs);
extern void isr_handler(registers_t regs);

void common_handler(registers_t regs)
{
// 	printf("Received interrupt: 0x%X (%d)\n", regs.int_no, regs.int_no);
	// Because I use the interrupt handler for the same as the
	// IRQ handler, we have to handle it here in C.
	if (!regs.is_irq)
		isr_handler(regs);
	else
		irq_handler(regs);
}
