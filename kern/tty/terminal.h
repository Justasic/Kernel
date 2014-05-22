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
#ifndef __VGA_TERMINAL_H__
#define __VGA_TERMINAL_H__
#include <stdbool.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum vga_colors
{
	COLOR_BLACK         = 0,
	COLOR_BLUE          = 1,
	COLOR_GREEN         = 2,
	COLOR_CYAN          = 3,
	COLOR_RED           = 4,
	COLOR_MAGENTA       = 5,
	COLOR_BROWN         = 6,
	COLOR_LIGHT_GREY    = 7,
	COLOR_DARK_GREY     = 8,
	COLOR_LIGHT_BLUE    = 9,
	COLOR_LIGHT_GREEN   = 10,
	COLOR_LIGHT_CYAN    = 11,
	COLOR_LIGHT_RED     = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN   = 14,
	COLOR_WHITE         = 15
};

// Our color type.
typedef uint8_t vga_color_t;

// Redraw the terminal -- same as vga_initialize
#define VGAClear VGAInitialize

// External functions.
extern void VGAWriteNString(const char *str, size_t len, vga_color_t color);
extern void VGAWriteRNString(const char *str, size_t len);
extern void VGAWriteRString(const char *str);
extern void VGAWriteString(const char* str, vga_color_t color);
extern void VGAInitialize(void);
extern void VGAPutc(char c, vga_color_t color);
extern void VGARedraw(void);
extern void VGABackground(vga_color_t color);

// set in the upper bits of the character.
// 0xFBCC - FB = foreground and Background; CC = ascii character
// static inline because clang follows C99 standard for inline functions.
// `- see http://stackoverflow.com/questions/10243018/inline-function-undefined-symbols-error
#define VGAColor(fg, bg) ((fg << 4) | (bg & 0x0F))
// static inline vga_color_t vga_color(uint8_t fg, uint8_t bg) { return (fg << 4) | (bg & 0x0F); }


#endif
