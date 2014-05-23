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

extern void InitializeDescriptorTables(void);

// Welcome to C. We have alcohol in the libc
// and there's chips in the IRQs. Be sure to
// go check out the VGA before you go home
// tonight. Oh and don't forget, have a good time.
// ;)
void KernelStart(uint32_t esp)
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
	
	// Initialize the keyboard before anything else, this can allow us
	// to process input in case we come to a condition on whether we
	// wanna continue or not.
	InitializeKeyboard();
	
	// print some basic CPU information, this will be moved away later.
	printf("CPU: %s\n", GetCPUVendor());
	printf("CPU has FPU: %b\n", CPUSupportsFeature(CPUID_FEAT_EDX_FPU));
	
	// Print our placement address
	printf("Placement allocations start at 0x%X\n", (uint32_t)&bin_end);
	
	// Install our interrupt handler so we can handle
	// CPU events properly.
	InitializeDescriptorTables();
	
	// Initialize the paging system
	InitializePaging();
	
	// Re-enable interrupts
	ExitCriticalSection();
	
	// Test our interrupt
// 	__asm__ __volatile__("int $0x80" :: "a" (0x10));
	
	// Initialize the Programmable Interrupt Timer at 100Hz
	InitializePIT(100);
	
	// Sleep for a bit
	sleep(4);
	
	printf("Done waiting!\n");
	
	char *str = kalloc(300);
	strcpy(str, "Test kalloc!");
	printf("%s\n", str);
	printf("Freeing string!\n");
	kfree(str);
	
	// If paging works correctly, this will call an
	// syscall which kills the kernel via a panic
// 	__asm__ __volatile__("int $0x80" :: "a" (0x10));
	
	// Idle loop to make sure we don't leave
	// the function and halt the CPU.
	// The 'hlt' opcode is so the CPU stops until
	// an event happens (notice we did not put 'sti'
	// before the hlt instruction?)
	while(true)
	{
		CleanHeaps();
		char *strs[10];
		for (uint32_t i = 0; i < 10; ++i)
		{
			char *str = kalloc(1024);
			char str2[] = "Really dirty memory block all over th eplace adsklfjhasdfjsahdfkjdaslfasdjkf\n\n\0asdkflj\radsf\0\r\n";
			memcpy(str, str2, sizeof(str2));
			strs[i] = str;
		}
		
		for (uint32_t i = 0; i < 10; ++i)
			kfree(strs[i]);
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
