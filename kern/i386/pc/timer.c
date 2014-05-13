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
#include <stdio.h>
#include "lib/common.h"
#include "i386/pc/isr.h"

uint32_t tick = 0;

static void callback(registers_t regs)
{
	tick++;
	printf("Tick: %d\r", tick);
}

void init_PIT(uint32_t freq)
{
	printf("Initializing timer with %u Hz\n", freq);
	// register our timer callback
	register_interrupt_handler(IRQ0, &callback);
	
	// The value we send to the PIT is the value to divide it's input clock
	// (1193180 Hz) by, to get our required frequency. Important to note is
	// that the divisor must be small enough to fit into 16-bits.
	uint32_t divisor = 1193180 / freq;
	
	// Send the command byte.
	outb(0x43, 0x36);
	
	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
	
	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
}
