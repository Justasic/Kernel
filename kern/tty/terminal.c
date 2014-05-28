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
#include "terminal.h"
#include <lib/common.h>
#include <ctype.h>

// Video memory pointer - always constant
static uint16_t *vidmem = (uint16_t *) 0xB8000;

// These are global string printing driver interfaces, required when initializing our terminal here
extern void (*print_color)(const char *, size_t len, vga_color_t color);
extern void (*print_rainbow)(const char *str, size_t len);
extern void (*putch)(const char c, vga_color_t color);

// The current columns and rows in the terminal.
uint8_t row;
uint8_t column;

// All VGA terminals are 80x24
#define VGA_WIDTH 80
#define VGA_HEIGHT 24

// Keep a buffer of all text on screen, allows redraws
uint16_t buffer[VGA_HEIGHT * VGA_WIDTH];

// A black-background, white-foreground space character.
const uint16_t blank = ((VGAColor(COLOR_BLACK, COLOR_WHITE) << 8) | ' ');

// Scrolls the text on the screen up by one line.
static void _vgaScroll(void)
{
	// Row 25 is the end, this means we need to scroll up
	if(row >= (VGA_HEIGHT+1))
	{
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0; i < (VGA_HEIGHT * VGA_WIDTH); i++)
		{
			vidmem[i] = vidmem[i+VGA_WIDTH];
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
void VGAPutc(char c, vga_color_t color)
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
	else if(isprint(c))
	{
		location = vidmem + (row * VGA_WIDTH + column);
		*location = c | (color << 8);
		column++;
	}
	
	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if (column >= VGA_WIDTH)
	{
		column = 0;
		row++;
	}
	
	// Scroll the screen if needed.
	_vgaScroll();
	// Move the hardware cursor.
	_vgaMoveCursor();
}

// Write a null-terminated string
void VGAWriteString(const char* str, vga_color_t color) { VGAWriteNString(str, strlen(str), color); }

// Write a string with a specific length
void VGAWriteNString(const char *str, size_t len, vga_color_t color)
{
	for (size_t i = 0; i < len; ++i)
		VGAPutc(str[i], color);
}

// Neat function to write rainbow strings
void VGAWriteRNString(const char *str, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		size_t co = i % COLOR_WHITE;
// 		if (co == COLOR_DARK_GREY || co == COLOR_LIGHT_BROWN)
// 			co += 2;
		if (co == COLOR_BLACK)
			co++; // Just a random color.
			
		// if it's an ascii charactor
		if (str[i] >= ' ')
			VGAPutc(str[i], VGAColor(COLOR_BLACK, (vga_color_t)co));
		else
			VGAPutc(str[i], VGAColor(COLOR_BLACK, VGAColor(COLOR_BLACK, COLOR_WHITE)));
	}
}

void VGAWriteRString(const char *str) { VGAWriteRNString(str, strlen(str)); }

// Initialize the VGA console
void VGAInitialize(void)
{
	// Check whether we're initialized.
	static uint8_t initialized = 0;
	row = column = 0;
	
	for (size_t i = 0; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
		vidmem[i] = blank;

	memset(&buffer, 0, VGA_HEIGHT * VGA_WIDTH);
	
	// Since we're an initialization function
	// as well as a clear function, we wanna be
	// able to initialize and clear without
	// initialization. So we check if we have
	// already been initialized.
	if (initialized == 0)
	{
		VGAWriteString("Initialized VGA console.\n", VGAColor(COLOR_BLACK, COLOR_WHITE));
		print_color = VGAWriteNString;
		print_rainbow = VGAWriteRNString;
		putch = VGAPutc;
		initialized = 1;
	}
}

// Redraw the console.
void VGARedraw(void)
{
	// Set all the characters to a blank space
	memset(vidmem, blank, VGA_HEIGHT * VGA_WIDTH);
	
	for (size_t i = 0; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
		vidmem[i] = buffer[i];

// 	for (size_t y = 0; y < VGA_HEIGHT; ++y)
// 	{
// 		for (size_t x = 0; x < VGA_WIDTH; ++x)
// 			vidmem[y * VGA_WIDTH + x] = buffer[y][x];
// 	}
	
	_vgaMoveCursor();
}

// Change the background and foreground color of all the text.
void VGABackground(vga_color_t color)
{
	for (int i = 0; i < (VGA_HEIGHT * VGA_WIDTH); i++)
	{
		// Get the char again
		uint8_t ch = vidmem[i] & 0xFF;
		vidmem[i] = ch | (color << 8);
	}
}
