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

#include "stdio.h"
#include "stdarg.h"
#include "limits.h"

// TODO: this is temporary to clear away some of the warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wreturn-type"


static char *base10(unsigned x, char *s)
{
	*--s = 0;
	if (!x)
		*--s = '0';
	for (; x; x/=10)
		*--s = '0'+ x % 10;
	return s;
}

static char *hex(unsigned x, char *s)
{
	unsigned num = 0;
	*--s = 0;
	if (!x)
		*--s = '0';
	for (; x; x/=16)
	{
		num = x % 16;
		if(num < 10)
			*--s = '0' + num;
		else
			*--s = 'A' + num - 10;
	}
	return s;
}

static char *bin(unsigned x, char *s)
{
	int iterations = sizeof(x) / CHAR_BIT;
// 	for (int )
}


int vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	char *save = str;
	int state = 0;
// 	va_start(ap, format);
	while (*format != 0)
	{
		switch (*format)
		{
			case '%':
			default:
				*str++ = *format++;
		}
	}
}

#pragma clang diagnostic pop
