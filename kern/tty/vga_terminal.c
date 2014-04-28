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
#include <string.h>
#include "vga_terminal.h"
#include <lib/common.h>

// Video memory pointer - always constant
static uint16_t *vidmem = (uint16_t *) 0xB8000;

// These are global string printing driver interfaces, required when initializing our terminal here
extern void (*print_color)(const char *, size_t len, vga_color_t color);

// The current columns and rows in the terminal.
uint8_t row;
uint8_t column;

// All VGA terminals are 80x24
#define VGA_WIDTH 80
#define VGA_HEIGHT 24

// Keep a buffer of all text on screen, allows redraws
uint16_t buffer[VGA_HEIGHT][VGA_WIDTH];

// Scrolls the text on the screen up by one line.
static void _vgaScroll(void)
{
	// A black-background, white-foreground space character.
	uint16_t blank = ((vga_color(COLOR_BLACK, COLOR_WHITE) << 8) | ' ');
	// Get a space character with the default color attributes.
// 	uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
// 	uint16_t blank = 0x20 /* space */ | (attributeByte << 8);
	
	// Row 25 is the end, this means we need to scroll up
	if(row >= (VGA_HEIGHT+1))
	{
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0; i < (VGA_HEIGHT * VGA_WIDTH); i++)
		{
			vidmem[i] = vidmem[i+VGA_HEIGHT];
		}
		
		// The last line should now be blank. Do this by writing
		// VGA_HEIGHT spaces to it.
		for (i = (VGA_HEIGHT * VGA_WIDTH); i < ((VGA_HEIGHT+1) * VGA_WIDTH); i++)
		{
			vidmem[i] = blank;
		}
		// The cursor should now be on the last line.
		row = 24;
	}
}

// Updates the hardware cursor.
static void _vgaMoveCursor(void)
{
	// The screen is VGA_WIDTH characters wide...
	uint16_t cursorLocation = row * VGA_WIDTH + column;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
	outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Put a character on the screen, scroll if needed
void vga_putc(char c, vga_color_t color)
{
	uint16_t *location;
	
	// Handle a backspace, by moving the cursor back one space
	if (c == 0x08 && column)
		column--;
	
	// Handle a tab by increasing the cursor's X, but only to a point
	// where it is divisible by 8.
	else if (c == 0x09)
		column = (column+8) & ~(8-1);
	
	// Handle carriage return
	else if (c == '\r')
		column = 0;
	
	// Handle newline by moving cursor back to left and increasing the row
	else if (c == '\n')
	{
		column = 0;
		row++;
	}
	// Handle any other printable character.
	else if(c >= ' ')
	{
		location = vidmem + (row * VGA_WIDTH + column);
		*location = c | color;
		column++;
	}
	
	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if (column >= VGA_WIDTH)
	{
		column = 0;
		row ++;
	}
	
	// Scroll the screen if needed.
	_vgaScroll();
	// Move the hardware cursor.
	_vgaMoveCursor();
#if 0
	vga_putc_at(c, color, column, row);
	
	// Make sure we're not going off screen
	if(++column == VGA_WIDTH)
	{
		column = 0;
		if(++row == VGA_HEIGHT)
			row = 0;
	}
#endif
}

// Write a null-terminated string
void vga_write_string(const char* str, vga_color_t color) { vga_write_nstring(str, strlen(str), color); }

// Write a string with a specific length
void vga_write_nstring(const char *str, size_t len, vga_color_t color)
{
	for(size_t i = 0; i < len; ++i)
		vga_putc(str[i], color);
}

// Initialize the VGA console
void vga_initialize(void)
{
#if 0
	row = column = 0;
	vga_color_t color = vga_color(COLOR_WHITE, COLOR_BLACK);
	
	for(size_t x = 0; x < VGA_WIDTH; x++)
	{
		for(size_t y = 0; y < VGA_HEIGHT; y++)
			vga_putc_at(' ', color, x, y);
	}
#else
	uint16_t blank = ((vga_color(COLOR_BLACK, COLOR_WHITE) << 8) | ' ');
	memset(vidmem, blank, VGA_HEIGHT * VGA_WIDTH);
#endif

	memset(&buffer, 0, VGA_HEIGHT * VGA_WIDTH);
	print_color = vga_write_nstring;
}

// Redraw the console.
void vga_redraw(void)
{
	// Set all the characters to a blank space
	uint16_t blank = ((vga_color(COLOR_BLACK, COLOR_WHITE) << 8) | ' ');
	memset(vidmem, blank, VGA_HEIGHT * VGA_WIDTH);

	for (size_t y = 0; y < VGA_HEIGHT; ++y)
	{
		for (size_t x = 0; x < VGA_WIDTH; ++x)
			vidmem[y * VGA_WIDTH + x] = buffer[y][x];
	}
	
	_vgaMoveCursor();
}
