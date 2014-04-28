/*
 * Copyright (c) 2013, Justin Crawford <Justasic@gmail.com>
 * Copyright (c) 2013, Piero Barbagelata <lordofsraam@hotmail.com>
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
#include <string.h>
#include <tty/vga_terminal.h>
#include <lib/panic.h>

static uint16_t *vidmem = (uint16_t *) 0xB8000;
static char *str;
extern uint32_t start_stack;
extern char *hex(unsigned x, char *s);
extern char *itoa(char *buffer, int num);

// Start here? can we do this? idk. I DECLARE 0xC0000 TO BE ALLOCATED!
//static char *buffer = (char*)0xC0000;
static char buffer[(1<<16)];

// As much as this goes against things with me, there is really no reason
// to return an integer.. Maybe in the future we can provide kernel launch
// arguments from the bootloader but for now? void kern_start(void).
void kern_start(uint32_t esp)
{
	start_stack = esp;
	
	//memset(buffer, 0, (1<<16)-1);
	
	str = itoa(buffer, esp);
	strcat(str, " Hello World!");
	
	vga_initialize();
	char /* *str = "Hello world!",*/ *ch;
	unsigned i;
	
	for(ch = str, i = 0; *ch; ch++, i++)
		vidmem[i] = (unsigned char) *ch | 0x0700; 
	
	panic("Kernel Execution End.");
	
	//print_str("Hello World", 0);
	
// 	vga_initialize();
// 	
// 	printf("Hello world!\n");
	
}

