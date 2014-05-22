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
######################################################
# This file defines the Global Interrupt Table (GDT)
# This is where all interrupts are handled and sent to
# a common interrupt handler in C which will actually
# handle whatever interrupt we're listening for.
######################################################

# this macro is pretty simple
# It defines a label for the interrupt number.
# It clears all maskabel interrupts (entering critical section)
# pushes the constant '0' to the stack (eg, the error code)
# pushes the interrupt number to the stack
# jumps to the common interrupt handler.
#define ISR_NOERRORCODE(x) \
	.globl isr##x; \
	isr##x: \
		cli; \
		push $0; \
		push $0; \
		push $##x; \
		jmp isr_stub;

# This macro is similar to the one
# above but the CPU has already pushed
# the error code to the stack and therefore
# we just need to define what interrupt
# is being called then jump to the stub.
#define ISR_ERRORCODE(x) \
	.globl isr##x; \
	isr##x: \
		cli; \
		push $0; \
		push $##x; \
		jmp isr_stub;
		
#define IRQ(x, y) \
	.globl irq##x; \
	irq##x: \
		cli; \
		push $0; \
		push $1; \
		push $##y; \
		jmp isr_stub;
		

# Our common stub for all interrupts which will prepare us
# to enter a C function to handle the interrupt.
isr_stub:
	pusha          # Pushes all registers to stack -- save while we change the data segment register
	
	movw %ds, %ax  # Move data segment register to eax
	push %eax      # Push eax (data segment reg) to the stack
	
	# Load the kernel data segment descriptor
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	
	# Jump to C and handle the interrupt
	call CommonHandler
	
	# Restore original data segment descriptor
	pop %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	# Pop all registers -- restore previous registers
	popa
	# Cleans up pushed error code and pushed interrupt number AND the 'boolean' value for whether it's an IRQ or not
	addl $12, %esp
	# Re-enable interrupts
	sti
	# Return to where we were when interrupt was called
	iret
	
# Interrupts 0-18
#
#
#    0 - Division by zero exception
#    1 - Debug exception
#    2 - Non maskable interrupt
#    3 - Breakpoint exception
#    4 - 'Into detected overflow'
#    5 - Out of bounds exception
#    6 - Invalid opcode exception
#    7 - No coprocessor exception
#    8 - Double fault (pushes an error code)
#    9 - Coprocessor segment overrun
#    10 - Bad TSS (pushes an error code)
#    11 - Segment not present (pushes an error code)
#    12 - Stack fault (pushes an error code)
#    13 - General protection fault (pushes an error code)
#    14 - Page fault (pushes an error code)
#    15 - Unknown interrupt exception
#    16 - Coprocessor fault
#    17 - Alignment check exception
#    18 - Machine check exception
#    19-31 - Reserved
#
ISR_NOERRORCODE(0)    # Division by zero exception
ISR_NOERRORCODE(1)    # Debug exception
ISR_NOERRORCODE(2)    # Non-maskable interrupt
ISR_NOERRORCODE(3)    # Breakpoint exception
ISR_NOERRORCODE(4)    # 'Intro detected overflow'
ISR_NOERRORCODE(5)    # Out of bounds exception
ISR_NOERRORCODE(6)    # Invalid opcode exception
ISR_NOERRORCODE(7)    # No coprocessor exception
ISR_ERRORCODE(8)      # Double fault (w/ error code)
ISR_NOERRORCODE(9)    # Coprocessor segment overrun
ISR_ERRORCODE(10)     # Bad TSS (w/ error code)
ISR_ERRORCODE(11)     # Segment not present (w/ error code)
ISR_ERRORCODE(12)     # Stack Fault (w/ error code)
ISR_ERRORCODE(13)     # General protection fault (w/ error code)
ISR_ERRORCODE(14)     # Page fault (w/ error code)
ISR_NOERRORCODE(15)   # Unknown interrupt exception
ISR_NOERRORCODE(16)   # Coprocessor fault
ISR_NOERRORCODE(17)   # Alignment check exception
ISR_NOERRORCODE(18)   # Machine check exception

# Reserved interrupts
ISR_NOERRORCODE(19)
ISR_NOERRORCODE(20)
ISR_NOERRORCODE(21)
ISR_NOERRORCODE(22)
ISR_NOERRORCODE(23)
ISR_NOERRORCODE(24)
ISR_NOERRORCODE(25)
ISR_NOERRORCODE(26)
ISR_NOERRORCODE(27)
ISR_NOERRORCODE(28)
ISR_NOERRORCODE(29)
ISR_NOERRORCODE(30)
ISR_NOERRORCODE(31)

# Syscall interrupt -- 0x80
ISR_NOERRORCODE(128) 

# Interrupt requests
IRQ(0, 32)
IRQ(1, 33)
IRQ(2, 34)
IRQ(3, 35)
IRQ(4, 36)
IRQ(5, 37)
IRQ(6, 38)
IRQ(7, 39)
IRQ(8, 40)
IRQ(9, 41)
IRQ(10, 42)
IRQ(11, 43)
IRQ(12, 44)
IRQ(13, 45)
IRQ(14, 46)
IRQ(15, 47)
