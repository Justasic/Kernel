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

################################################################
# Normally, I would have written this in C but because
# the compiler (even with something like __fastcall) refused
# to not use the stack or dereference a memory location when
# paging is disabled (thus causing CPU exceptions and all kinds
# of awful things). So. Here I am writing a fucking function myself
# because compilers are too stupid to understand how to do things
# in C the way I want.
#################################################################


.globl ClonePhysicalPage
ClonePhysicalPage:
	pushl %ebx             # According to __cdecl, we must preserve the contents of EBX.
	pushf                  # push EFLAGS, so we can pop it and reenable interrupts
			       # later, if they were enabled anyway.
	cli                    # Disable interrupts, so we aren't interrupted.
			       # Load these in BEFORE we disable paging!
	mov 12(%esp), %ebx     # Source address
	mov 16(%esp), %ecx     # Destination address

	call DisablePaging     # Disable Paging

	movl $1024, %edx       # 1024*4bytes = 4096 bytes to copy

.loop:
	movl (%ebx), %eax      # Get the word at the source address
	movl %eax, (%ecx)      # Store it at the dest address
	addl $4, %ebx          # Source address += sizeof(word)
	addl $4, %ecx          # Dest address += sizeof(word)
	decl %edx              # One less word to do
	jnz .loop

	call EnablePaging      # Enable paging again
	
	sti                    # Enable interrupts again

	popf                   # Pop EFLAGS back.
	popl %ebx              # Get the original value of EBX back.
	ret
