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
#include "string.h"
#include "vga_terminal.h"

// Video memory pointer - always constant
static uint16_t *vidmem = (uint16_t *) 0xB8000;

size_t row;
size_t column;

// All VGA terminals are 80x24
#define VGA_WIDTH 80
#define VGA_HEIGHT 24

// Keep a buffer of all text on screen, allows redraws
char buffer[VGA_HEIGHT][VGA_WIDTH];
// Plain text buffer - no colors
char ptbuffer[VGA_HEIGHT][VGA_WIDTH];

void vga_putc_at(char c, vga_color_t color, size_t x, size_t y)
{
	// make the color and the char one variable and place it in the vmem
	uint16_t char16 = c, color16 = color;
	vidmem[y * VGA_WIDTH + x] = (char16 | color16 << 8);
	
	// Insert into buffers - probably unsafe but whatever.
	buffer[x][y] = (char16 | color16 << 8);
	ptbuffer[x][y] = c | 0x07000;
}

void vga_putc(char c, vga_color_t color)
{
	vga_putc_at(c, color, column, row);
	
	// Make sure we're not going off screen
	if(++column == VGA_WIDTH)
	{
		column = 0;
		if(++row == VGA_HEIGHT)
			row = 0;
	}
}

void vga_write_string(const char* str, vga_color_t color)
{
	size_t len = strlen(str), i;
	for(i = 0; i < len; ++i)
		vga_putc(str[i], color);
}

vga_color_t vga_color(enum vga_color fg, enum vga_color bg)
{
	// set in the upper bits of the character.
	// 0xFBCC - FB = foreground and Background; CC = ascii character
	return fg | bg << 4;
}

// These are global string printing driver interfaces, required when initializing our terminal here
extern void (*print_color)(const char *, size_t len, vga_color_t color);

// This is used to write a color to a string of text.
void vga_print_color(const char *str, size_t len, uint32_t color)
{
	// Convert the color from 32 bit integer to an 8 bit integer
	vga_color_t color8 = (color >> 24);
	
	vga_write_string(str, color8);
}

// Initialize the VGA console
void vga_initialize(void)
{
	row = column = 0;
	vga_color_t color = vga_color(COLOR_WHITE, COLOR_BLACK);
	
	for(size_t x = 0; x < VGA_WIDTH; x++)
	{
		for(size_t y = 0; y < VGA_HEIGHT; y++)
			vga_putc_at(' ', color, x, y);
	}

	memset(&buffer, 0, VGA_HEIGHT * VGA_WIDTH);
	memset(&ptbuffer, 0, VGA_HEIGHT * VGA_WIDTH);
	print_color = vga_print_color;
}

// Clear the VGA console
void vga_clear(void)
{
	vga_initialize();
}

// Redraw the console.
void vga_redraw(bool plaintext)
{
	// Clear the entire terminal
	vga_color_t color = vga_color(COLOR_WHITE, COLOR_BLACK);
	
	size_t x, y;
	// Set all the characters to black on white spaces
	for(y = 0; y < VGA_HEIGHT; y++)
		for(x = 0; x < VGA_WIDTH; x++)
			vga_putc_at(' ', color, x, y);

	// If we want plain text (eg, black on white) use this.
	if(plaintext)
	{
		for(y = 0; y < VGA_HEIGHT; ++y)
			for(x = 0; x < VGA_WIDTH; ++x)
				vga_putc(ptbuffer[y][x], color);
	}
	else // If we're just redrawing for some reason.
		for(y = 0; y < VGA_HEIGHT; ++y)
			for(x = 0; x < VGA_WIDTH; ++x)
				vidmem[y * VGA_WIDTH + x] = ptbuffer[y][x];
}
