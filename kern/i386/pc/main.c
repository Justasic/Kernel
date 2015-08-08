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
#include "mm/kmemory.h"
#include "mm/paging.h"
// #include "lib/linux.h"
#include "tty/terminal.h"
#include "input/keyboard.h"
#include "i386/pc/cpuid.h"
#include "i386/pc/timer.h"
#include "i386/pc/multiboot.h"

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

// This is one ugly-ass macro.
#define BNYEH_LOGO \
" ____                            __         \n" \
"/\\  _`\\                         /\\ \\        \n" \
"\\ \\ \\L\\ \\    ___   __  __     __\\ \\ \\___    \n" \
" \\ \\  _ <' /' _ `\\/\\ \\/\\ \\  /'__`\\ \\  _ `\\  \n" \
"  \\ \\ \\L\\ \\/\\ \\/\\ \\ \\ \\_\\ \\/\\  __/\\ \\ \\ \\ \\ \n" \
"   \\ \\____/\\ \\_\\ \\_\\/`____ \\ \\____\\\\ \\_\\ \\_\\\n" \
"    \\/___/  \\/_/\\/_/`/___/> \\/____/ \\/_/\\/_/\n" \
"                       /\\___/               \n" \
"                       \\/__/                \n\n"

// defined by layout.ld
extern uint32_t start_stack;
extern uint32_t bin_end;
extern uint32_t comment;

extern void InitializeDescriptorTables(void);

// Welcome to C. We have alcohol in the libc
// and there's chips in the IRQs. Be sure to
// go check out the VGA before you go home
// tonight. Oh and don't forget, have a good time.
// ;)
void KernelStart(multiboot_t *mboot, uint32_t esp)
{
	start_stack = esp;

	// Do a quick check to make sure we're not running
	// on linux. If we are, print some message to the console
	// and exit. This calls the linux syscall for getpid.
#if 0
	if (LinuxCheck())
	{
		LinuxPuts(BNYEH_LOGO);
		LinuxPuts("Bnyeh kernel version 1.0.0\n");
		LinuxError("You dumbass, I am a kernel, not a program. Run me in a virtual machine.\n");
	}
#endif

	// Initialize the VGA console so we can
	// have messages printed to the terminal.
	VGAInitialize();

	// Logo! :D
	printf("Welcome to the Bnyeh Kernel!\n");
	printrf(BNYEH_LOGO);

	// Install our interrupt handler so we can handle
	// CPU events properly.
	InitializeDescriptorTables();

	// Re-enable interrupts
	ExitCriticalSection();

	// Initialize the keyboard before anything else, this can allow us
	// to process input in case we come to a condition on whether we
	// wanna continue or not.
	InitializeKeyboard();

	// Print our placement address
	printf("Placement allocations start at 0x%X\n", (uint32_t)&bin_end);

	printf("Compiled with: %s\n", (const char *)&comment);
	printf("Booted with: %s\n", (const char*)&mboot->boot_loader_name);

	// Detect the amount of memory we have

	// print some basic CPU information, this will be moved away later.
	GetCPUInfo();

	// Initialize the Programmable Interrupt Timer at 100Hz
	InitializePIT(100);

	// Initialize the paging system
 	InitializePaging();

	// Make sure we reenable interrupts.
	ExitCriticalSection();

	// Test our interrupt
// 	__asm__ __volatile__("int $0x80" :: "a" (0x10));

	printf("Sleep before test\n");
	// Sleep for a bit
	sleep(4);

	printf("Done waiting!\n");

	char *str = kalloc(300);
	sleep(2);
	strcpy(str, "Test kalloc!");
	sleep(2);
	printf("%s\n", str);
	sleep(2);
	printf("Freeing string!\n");
	sleep(2);
	kfree(str);

	/*printf("Page fault.\n");
	int *ptr = 0x12123123;
	*ptr = 1;*/

	// If paging works correctly, this will call an
	// syscall which kills the kernel via a panic
// 	__asm__ __volatile__("int $0x80" :: "a" (0x10));

	// Idle loop to make sure we don't leave
	// the function and halt the CPU.
	// The 'hlt' opcode is so the CPU stops until
	// an event happens (notice we did not put 'sti'
	// before the hlt instruction?)
	printf("Entering idle-wait loop\n");
	while(true)
	{
		//CleanHeaps();
		__asm__ __volatile__("hlt");
	}
}

void KernelCleanup(void)
{
	// Wipe out the memory and then exit.
// 	uint32_t *mem_start = (uint32_t*)&bin_end;
	// NOTE: because we don't yet know the length of the ram
	// we cannot clear it all. We must detect the total ram in
	// the system.
// 	explicit_bzero(mem_start, );

}
