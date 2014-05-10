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
#include <stdlib.h>
#include <stddef.h>

#include "lib/panic.h"
#include "lib/common.h"
#include "tty/vga_terminal.h"
#include "i386/pc/cpuid.h"
#include "i386/pc/timer.h"

#ifdef __LINUX__
# warning "This kernel is being compiled with a non-cross-compiler!"
#endif

#if 0
" ____                            __         "
"/\  _`\                         /\ \        "
"\ \ \L\ \    ___   __  __     __\ \ \___    "
" \ \  _ <' /' _ `\/\ \/\ \  /'__`\ \  _ `\  "
"  \ \ \L\ \/\ \/\ \ \ \_\ \/\  __/\ \ \ \ \ "
"   \ \____/\ \_\ \_\/`____ \ \____\\\ \_\ \_\"
"    \/___/  \/_/\/_/`/___/> \/____/ \/_/\/_/"
"                       /\___/               "
"                       \/__/                "
#endif

extern uint32_t start_stack;

extern void initialize_descriptor_tables(void);

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
	printf("Welcome to the Bnyeh Kernel!\n");
	printrf(
// 		vga_color(COLOR_BLACK, COLOR_GREEN),
		" ____                            __         \n"
		"/\\  _`\\                         /\\ \\        \n"
		"\\ \\ \\L\\ \\    ___   __  __     __\\ \\ \\___    \n"
		" \\ \\  _ <' /' _ `\\/\\ \\/\\ \\  /'__`\\ \\  _ `\\  \n"
		"  \\ \\ \\L\\ \\/\\ \\/\\ \\ \\ \\_\\ \\/\\  __/\\ \\ \\ \\ \\ \n"
		"   \\ \\____/\\ \\_\\ \\_\\/`____ \\ \\____\\\\ \\_\\ \\_\\\n"
		"    \\/___/  \\/_/\\/_/`/___/> \\/____/ \\/_/\\/_/\n"
		"                       /\\___/               \n"
		"                       \\/__/                \n\n"
	);
	
	printf("CPU: %s\n", GetCPUVendor());
	printf("CPU has FPU: %b\n", CPUSupportsFeature(CPUID_FEAT_EDX_FPU));
	
	// Install our interrupt handler so we can handle
	// CPU events properly.
	initialize_descriptor_tables();
	
	// Test printf, will be removed later.
	int i = printf("Test Printf!\n");
	printf("Previous printf returned %d\n", i);
	
	// Re-enable interrupts
// 	ExitCriticalSection();
	
// 	__asm__ __volatile__("int $0x80");
	
	// Initialize the Programmable Interrupt Timer at 50Hz
// 	init_PIT(50);

	
	panic("Kernel Execution End.", NULL);
}

