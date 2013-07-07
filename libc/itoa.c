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

char *itoa(char *buffer, int num)
{
	//char *buffer = malloc(2*sizeof(num)+1);
	// if negative, need 1 char for the sign
	int sign = (num < 0) ? true : false;
	int i = 0;
	if (num == 0)
		buffer[i++] = '0';
	else if (num < 0) {
		buffer[i++] = '-';
		num = -num;
	}
	
	while (num > 0) {
		buffer[i++] = '0' + num % 10;
		num /= 10;
	}
	
	buffer[i] = '\0';
	strnrev(buffer + sign, i - sign);
	
	return 0;
}
