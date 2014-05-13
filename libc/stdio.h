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
#ifndef __STDIO_H__
#define __STDIO_H__
#include "stdarg.h"
#include "stdint.h"
#include <tty/vga_terminal.h>

#define ___ATTRIB_FORMAT__(x, y) __attribute__((format(printf, x, y)))


___ATTRIB_FORMAT__(1, 2) extern int printf(const char *, ...);
___ATTRIB_FORMAT__(1, 2) extern int printrf(const char *fmt, ...);
___ATTRIB_FORMAT__(2, 3) extern int printcf(uint32_t color, const char *fmt, ...);
___ATTRIB_FORMAT__(2, 3) extern int sprintf(char *str, const char *format, ...);
___ATTRIB_FORMAT__(3, 4) extern int snprintf(char *str, size_t size, const char *format, ...);


extern int vsprintf(char *str, const char *format, va_list ap);
extern int vsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif
