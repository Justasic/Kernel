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
#include "i386/pc/isr.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
	printf("Installing interrupt handler %d\n", n);
	interrupt_handlers[n] = handler;
}

void irq_handler(registers_t regs)
{
	// err_code is the IRQ number.
	printf("Received IRQ: %d\n", regs.int_no);
	
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.int_no >= 40)
	{
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
	
	// Send reset signal to master. (As well as slave, if necessary).
	outb(0x20, 0x20);
	
	if (interrupt_handlers[regs.int_no] != 0)
	{
		printf("Calling IRQ handler for IRQ %d\n", regs.int_no);
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	for(int i = 0; i < 10000; ++i)
		++i;
}
