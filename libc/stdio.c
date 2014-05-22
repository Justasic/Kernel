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

#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include <tty/terminal.h>
// #include <lib/kmemory.h>

// TODO: this is temporary to clear away some of the warnings
// #warning "Developer notice: This file is incomplete."
// #pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Wunused-variable"
// #pragma clang diagnostic ignored "-Wunused-parameter"
// #pragma clang diagnostic ignored "-Wunused-function"
// #pragma clang diagnostic ignored "-Wreturn-type"

// Prints colored and non-colored strings, these should be initialized when the kernel starts.
// This is defined to allow for re-initialization by a better driver later in
// the kernel init stages.
void (*print_color)(const char *, size_t len, vga_color_t color);
void (*print_rainbow)(const char *str, size_t len);
void (*putch)(const char c, vga_color_t color);

// Print formatted
___ATTRIB_FORMAT__(1, 2)
int printf(const char *fmt, ...)
{
	// Call vsnprintf instead of decoding here, saves function calls.
	if (!print_color)
		return -1;
	
	// guess memory size.
// 	size_t newlen = strlen(fmt) * 2;
// 	char *str = malloc(newlen);
	// FIXME: Since we don't quite have memory allocation yet,
	// allocate a buffer on the stack which will handle *MOST*
	// printf messages BUT NOT ALL!
	char str[(1 << 10)]; // 65536 chars
	
	va_list ap;
	va_start(ap, fmt);
	
	//vsnprintf(str, newlen, fmt, ap);
	int ret = vsnprintf(str, sizeof(str), fmt, ap);
	// Null-terminate
	str[ret] = 0;
	
	va_end(ap);
	
	// NOTE: we use ret here instead of sizeof(str) otherwise we're going
	// to basically start catting our kernel .text segment to the terminal
	// (which is nice and all but we just wanted to say hello world..)
	if (print_color)
		print_color(str, ret, VGAColor(COLOR_BLACK, COLOR_LIGHT_GREY));
	
	return ret;
// 	free(str);
}

// Print formatted color
___ATTRIB_FORMAT__(2, 3)
int printcf(uint32_t color, const char *fmt, ...)
{
	// Call vsnprintf instead of decoding here, saves function calls.
	if (!print_color)
		return -1;
	
	// guess memory size.
	// 	size_t newlen = strlen(fmt) * 2;
	// 	char *str = malloc(newlen);
	// FIXME: Since we don't quite have memory allocation yet,
	// allocate a buffer on the stack which will handle *MOST*
	// printf messages BUT NOT ALL!
	char str[(1 << 10)]; // 65536 chars
	
	va_list ap;
	va_start(ap, fmt);
	
	//vsnprintf(str, newlen, fmt, ap);
	int ret = vsnprintf(str, sizeof(str), fmt, ap);
	// Null-terminate
	str[ret] = 0;
	
	va_end(ap);
	
	// NOTE: we use ret here instead of sizeof(str) otherwise we're going
	// to basically start catting our kernel .text segment to the terminal
	// (which is nice and all but we just wanted to say hello world..)
	if (print_color)
// 		print_color(str, newlen, VGAColor(COLOR_BLACK, COLOR_WHITE));
		print_color(str, ret, color);
	
	return ret;
// 	free(str);
}

// Print formatted rainbow
___ATTRIB_FORMAT__(1, 2)
int printrf(const char *fmt, ...)
{
	// Call vsnprintf instead of decoding here, saves function calls.
	if (!print_rainbow)
		return -1;
	
	// guess memory size.
	// 	size_t newlen = strlen(fmt) * 2;
	// 	char *str = malloc(newlen);
	// FIXME: Since we don't quite have memory allocation yet,
	// allocate a buffer on the stack which will handle *MOST*
	// printf messages BUT NOT ALL!
	char str[(1 << 10)]; // 65536 chars
	
	va_list ap;
	va_start(ap, fmt);
	
	//vsnprintf(str, newlen, fmt, ap);
	int ret = vsnprintf(str, sizeof(str), fmt, ap);
	// Null-terminate
	str[ret] = 0;
	
	va_end(ap);
	
	// NOTE: we use ret here instead of sizeof(str) otherwise we're going
	// to basically start catting our kernel .text segment to the terminal
	// (which is nice and all but we just wanted to say hello world..)
	if (print_rainbow)
		print_rainbow(str, ret);
	
	return ret;
	// 	free(str);
}

___ATTRIB_FORMAT__(3, 4)
int snprintf(char *str, size_t size, const char *format, ...)
{
	// Call vsnprintf instead of decoding here, saves function calls.
	if (!str)
		return -1;
	
	va_list ap;
	va_start(ap, format);
	
	int ret = vsnprintf(str, size, format, ap);
	// Null-terminate
	str[ret] = 0;
	
	va_end(ap);
	
	return ret;
}

int vsprintf(char *str, const char *format, va_list ap)
{
	if (!format)
		return -1;
	
// 	char *str = stdio_malloc(1 << 16); // 65536 chars
	// WARNING: Guess the buffer. THIS IS BAD!!!
	int ret = vsnprintf(str, 1 << 16, format, ap);
	// Null-terminate
	str[ret] = 0;
	return ret;
}

___ATTRIB_FORMAT__(2, 3)
int sprintf(char *str, const char *format, ...)
{
	// The joy of having our own Libc is that because we defined malloc,
	// we can determine the max size of the string without killing the
	// entire kernel due to a memory fault.
	
	int ret = 0;
	va_list ap;
	va_start(ap, format);
	
	// WARNING: Guess the buffer. THIS IS BAD!!!
	ret = snprintf(str, 1 << 16, format, ap);
	// Null-terminate
	str[ret] = 0;
	
	va_end(ap);
	
	return ret;
	
	// 
	// return sprintf(str, <size>, format, ap);
// 	str = stdio_malloc(1 << 16); // 65536 chars

}

int putc(int c)
{
	putch(c, VGAColor(COLOR_BLACK, COLOR_LIGHT_GREY));
	return c;
}

// #pragma clang diagnostic pop
