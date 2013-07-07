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
#include "stdlib.h"
#include "ctype.h"
#include "stdbool.h"
#include "string.h"

// Thanks very much to piero for this.
char *itoa(char *buffer, int num)
{
	int m, i;
	bool neg = false;
	if (num < 0)
	{
		num = -num;
		neg = true;
	}
	
	for (m = 10, i = 0; num > 0; m *= 10, i++)
	{
		if (m > 10)
		{
			buffer[i] = (num % m) / (m / 10);
			num = num - (num % m);
		}
		else
			num = num - (buffer[i] = num % m);
		buffer[i] += '0';
	}
	
	if(neg)
		strcat(buffer, "-");
	
	return strrev(buffer);
}
