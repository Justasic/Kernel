/*
 * Copyright (c) 2013, Alyx Wolcott <contact@alyxw.me>
 * Copyright (c) 2013 - 2014, Justin Crawford <Justasic@gmail.com>
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
# This assembly file is as basic as it gets, the bootloader will call this file directly
# This file should correctly handle finding and organizing the main kernel arguments from
# the bootloader as well as calling kern_start with those arguments.

# See the link below for more on this bootstrapper.. maybe i will modify it beyond this
# but for now, i'll use theirs
# http://wiki.osdev.org/Bare_Bones#Bootstrap_Assembly

.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
# 'magic number' aka '1 bad boot' http://en.wikipedia.org/wiki/Hexspeak
.set MAGIC,    0x1BADB002       # lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard. We put this into a special
# section so we can force the header to be in the start of the final program.
# You don't need to understand all these details as it is just magic values that
# is documented in the multiboot standard. The bootloader will search for this
# magic sequence and recognize us as a multiboot kernel.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Currently the stack pointer register (esp) points at anything and using it may
# cause massive harm. Instead, we'll provide our own stack. We will allocate
# room for a small temporary stack by creating a symbol at the bottom of it,
# then allocating 16384 bytes for it, and finally creating a symbol at the top.
# the stack will grow up with a max size of 16 KiB
.section .bootstrap_stack
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The linker script specifies _bootstrap as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.
.section .text
.globl _bootstrap
.type _bootstrap, @function
_bootstrap:
	# Welcome to kernel mode!
	# Currently, we're in protected mode if this is compiled as an ELF binary
	#. If we're not an elf binary then we must initialize from Real Mode to
	# protected mode and then jump to C, otherwise C is rather useless.

	# C requires a stack to work on so allocate a small one
	# to start with and point it to the top.
	# (This will grow downwards as the system initializes and uses
	# more memory)
	movl $stack_top, %esp
	push %esp
	push %ebx

	# Jump to C! :D
	call KernelStart

	# Clean up the stack for the next call
	addl $8, %esp

	# Cleanup after the kernel is ready to leave.
	call KernelCleanup

	## If kern_start exits, we're gonna wanna hang the CPU.
	# We can't poweroff the computer because that requires
	# ACPI interface shit which I hear is really annoying
	# to implement.
	cli   # Clear all maskable interrupts - ignore all maskable interrupts basically
	hlt   # Halt the CPU until the next NMI arrives

	## if the CPU still does not hang, do an endless jump loop, retrying the halt each time.
.sysHang:
	cli
	hlt
	jmp .sysHang



# Set the size of the _bootstrap symbol to the current location '.' minus its start.
# This is useful when debugging or when call tracing is implemented.
.size _bootstrap, . - _bootstrap
