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

#ifdef __LINUX__
# warning "This kernel is being compiled with a non-cross-compiler!"
#endif

extern uint32_t start_stack;

// Define this here for now, needs to be in a new file
void isr_handler() { }

// As much as this goes against things with me, there is really no reason
// to return an integer.. Maybe in the future we can provide kernel launch
// arguments from the bootloader but for now? void kern_start(void).
void kern_start(uint32_t esp)
{
	start_stack = esp;
	
	// Initialize the VGA console so we can
	// have messages printed to the terminal.
	vga_initialize();
	
	vga_write_string("Hello World Test :D!\n", vga_color(COLOR_BLACK, COLOR_WHITE));
	
	
// 	panic("Kernel Execution End.");
}

