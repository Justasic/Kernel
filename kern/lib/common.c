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
#include "common.h"

// Write byte to port
void outb(uint16_t port, uint8_t value)
{
	__asm__ __volatile__("outb %1, %0" :: "dN" (port), "a" (value));
}

// Read Byte from port
uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

// read word (2 bytes) from port
uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__ __volatile__("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

// Write word (2 bytes) to port
void outw(uint16_t port, uint16_t value)
{
	__asm__ __volatile__("outw %1, %0" :: "dN" (port), "a" (value));
}

// Read double-word (4 bytes) from port
uint32_t inl(uint16_t port)
{
	uint32_t ret;
	__asm__ __volatile__("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void outl(uint16_t port, uint32_t value)
{
	__asm__ __volatile__("outl %1, %0" :: "dN" (port), "a" (value));
}

void sleep(uint32_t secs)
{
	extern uint32_t tick;
	uint32_t nexttick = tick + (secs * 100);
	while (tick < nexttick)
		__asm__ __volatile__("hlt");
}
