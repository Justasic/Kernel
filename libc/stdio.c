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
#include <tty/vga_terminal.h>


// Prints colored and non-colored strings, these should be initialized when the kernel starts.
// This is defined to allow for re-initialization by a better driver later in
// the kernel init stages.
void (*print_color)(const char *, size_t len, vga_color_t color);

// Print formatted
int printf(const char *fmt, ...)
{
	// Call vsnprintf instead of decoding here, saves function calls.
// 	vsnprintf(char *str, size_t size, const char *format, va_list ap)
	if (!print_color)
		return -1;
	
	// guess memory size.
	size_t newlen = strlen(fmt) * 2;
	char *str = malloc(newlen);
	
	va_list ap;
	va_start(ap, fmt);
	
	vsnprintf(str, newlen, fmt, ap);
	
	va_end(ap);
	
	if (!print_color)
		print_color(str, newlen, vga_color(COLOR_WHITE, COLOR_BLACK));
		
	free(str);
}

// Print formatted color
int printcf(const char *fmt, uint32_t color, ...)
{
	
}

int snprintf(char *str, size_t size, const char *format, ...)
{
	
}

int vsprintf(char *str, const char *format, va_list ap)
{
	
}

int sprintf(char *str, const char *format, ...)
{
	// The joy of having our own Libc is that because we defined malloc,
	// we can determine the max size of the string without killing the
	// entire kernel due to a memory fault.
	
	// 
	// return sprintf(str, <size>, format, ap);
}
