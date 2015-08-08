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

/* The bootloader will look at this image and start execution at the symbol
 *  designated as the entry point. */
ENTRY(_bootstrap)

/* Tell where the various sections of the object files will be put in the final
 *  kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	 *   loaded at by the bootloader. */
	. = 1M;
	
	/* First put the multiboot header, as it is required to be put very early
	 *   early in the image or the bootloader won't recognize the file format.
	 *   Next we'll put the .text section. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
	
	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
		/* Comment is defined by the compiler automatically
		   The + 1 is because the string starts with a null
		   character so we must iterate one byte past it.   */
		comment = . + 1;
		*(.comment)
		
		/* The string and symbol tables defined by the linker */
		symtable = .;
		*(.symtab)
		/* Again, + 1 to iterate past null. */
		strtable = . + 1;
		*(.strtab)
		/* Get the sizes of things */
		strtablesz = strtable - .;
		symtablesz = symtable - strtable;
	}
	
	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
	
	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	
	/* The compiler may produce other sections, by default it will put them in
	 *   a segment with the same name. Simply add stuff here as needed. */
	
	/* used to find the end of the binary so we can start memory
	 * allocation after this point. This is later used in kmemory.c */
	bin_end = .;
}
