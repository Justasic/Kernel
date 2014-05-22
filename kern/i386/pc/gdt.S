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

.globl FlushGDT # export as symbol for C
FlushGDT:
	movl 4(%esp), %eax # get the pointer to GDT
	lgdt (%eax)        # load the new GDT pointer
	
	movw $0x10, %ax     # 0x10 is the offset to the GDT to our data segment
	movw %ax, %ds      # Load all the data segment selectors
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	ljmp $0x08,$flush    # 0x08 is the offset to our code segment: Far jump!
flush:
	ret

.globl FlushIDT # export as symbol for C
FlushIDT:
	movl 4(%esp), %eax # Get the pointer to the IDT from stack, passed as a parameter. 
	lidt (%eax)        # Load the IDT pointer.
	ret
