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

#include "keyboard.h"
#include "lib/common.h"
#include "i386/pc/isr.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

const uint8_t usascii_kbd[128] = 
{
	// First row of keys on the keyboard
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	// Second row of keys
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	// Third row of keys
	0 /* ctrl */, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, /* lshift */
	// Forth row of keys
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, /* rshift */
	'*',
	0,   // Alt
	' ', // space
	0,   // caps lock
	// f1 - f10 keys
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,   // num lock
	0,   // scroll lock
	0,   // home key
	0,   // arrow key - Up
	0,   // page up
	'-', // dash key
	0,   // arrow key - left
	0,   // ?
	0,   // arrow key - right
	'+', // plus key
	0,   // end key
	0,   // arrow key - down
	0,   // page down
	0,   // insert key
	0,   // delete key
	// ?
	0, 0, 0,
	0,   // F11 key
	0,   // F12 key
	0,   // anything else is undefined.
};

const uint8_t usascii_kbd_shift[128] = 
{
	// First row of keys on the keyboard
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	// Second row of keys
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	// Third row of keys
	0 /* ctrl */, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, /* lshift */
	// Forth row of keys
	'|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, /* rshift */
	'*',
	0,   // Alt
	' ', // space
	0,   // caps lock
	// f1 - f10 keys
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,   // num lock
	0,   // scroll lock
	0,   // home key
	0,   // arrow key - Up
	0,   // page up
	'-', // dash key
	0,   // arrow key - left
	0,   // ?
	0,   // arrow key - right
	'+', // plus key
	0,   // end key
	0,   // arrow key - down
	0,   // page down
	0,   // insert key
	0,   // delete key
	// ?
	0, 0, 0,
	0,   // F11 key
	0,   // F12 key
	0,   // anything else is undefined.
};


bool shift = false;
bool caps = false;

void HandleKeyboardInput(registers_t __unused regs)
{
	// Get keyboard input
	uint8_t scancode = inb(0x60);
	
	// If the upper bit of the keyboard is set then the key was released
	if (scancode & 0x80)
	{
		// handle shift, ctrl, alt, etc.
// 		printf("Key released: 0x%X\n", scancode);
		
		// if the shift key is released
		if (scancode == 0xAA || scancode == 0xB6)
			shift = false;
	}
	else
	{
// 		printf("Key pressed: 0x%X\n", scancode);
		
		// If shift is pressed down
		if (scancode == 0x2A || scancode == 0x36)
			shift = true;
		
		if (scancode == 0x3A)
			caps = !caps;
		// key was just pressed, print the char
		//to the screen for now.
		if (shift)
		{
			// This emulates the caps lock on + shift behavior
			
			if (caps)
				// This lets us use the symbol keys but lower-case letters
				putc(tolower(usascii_kbd_shift[scancode]));
			else 
				putc(usascii_kbd_shift[scancode]);
		}
		else
		{
			if (caps)
				putc(toupper(usascii_kbd[scancode]));
			else
				putc(usascii_kbd[scancode]);
		}
	}
}

void InitializeKeyboard(void)
{
	printf("Initizing keyboard.\n");
	
	RegisterIRQHandler(IRQ1, &HandleKeyboardInput);
}
