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

#if 0
" ____                            __         "
"/\  _`\                         /\ \        "
"\ \ \L\ \    ___   __  __     __\ \ \___    "
" \ \  _ <' /' _ `\/\ \/\ \  /'__`\ \  _ `\  "
"  \ \ \L\ \/\ \/\ \ \ \_\ \/\  __/\ \ \ \ \ "
"   \ \____/\ \_\ \_\/`____ \ \____\\\\ \_\ \_\\"
"    \/___/  \/_/\/_/`/___/> \/____/ \/_/\/_/"
"                       /\___/               "
"                       \/__/                "
#endif

extern uint32_t start_stack;

// Define this here for now, needs to be in a new file
void isr_handler() { }

// Welcome to C. We have alcohol in the libc
// and there's chips in the IRQs. Be sure to
// go check out the VGA before you go home
// tonight. Oh and don't forget, have a good time.
// ;)
void kern_start(uint32_t esp)
{
	start_stack = esp;
	
	// Initialize the VGA console so we can
	// have messages printed to the terminal.
	vga_initialize();
	
	// Logo! :D
	vga_write_string("Welcome to the Bnyeh Kernel!\n", vga_color(COLOR_BLACK, COLOR_WHITE));
	vga_write_string(
		" ____                            __         \n"
		"/\\  _`\\                         /\\ \\        \n"
		"\\ \\ \\L\\ \\    ___   __  __     __\\ \\ \\___    \n"
		" \\ \\  _ <' /' _ `\\/\\ \\/\\ \\  /'__`\\ \\  _ `\\  \n"
		"  \\ \\ \\L\\ \\/\\ \\/\\ \\ \\ \\_\\ \\/\\  __/\\ \\ \\ \\ \\ \n"
		"   \\ \\____/\\ \\_\\ \\_\\/`____ \\ \\____\\\\ \\_\\ \\_\\\n"
		"    \\/___/  \\/_/\\/_/`/___/> \\/____/ \\/_/\\/_/\n"
		"                       /\\___/               \n"
		"                       \\/__/                \n\n",
		vga_color(COLOR_BLACK, COLOR_GREEN)
	);
	
	vga_write_string("Hello World Test :D!\n", vga_color(COLOR_BLACK, COLOR_WHITE));
	vga_write_string("This is a red string!\n", vga_color(COLOR_BLACK, COLOR_LIGHT_RED));
	vga_write_rstring("FUCKIN RAINBOW!\n");
	
// 	panic("Kernel Execution End.");
}

