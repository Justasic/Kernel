/*
 * Copyright (c) 2013, Alyx Wolcott <contact@alyxw.me>
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
#include "stddef.h"
#include "ctype.h"
#include "private/platform.h"

size_t strlen(const char *str)
{
	size_t len = 0;
	while(*str++)
		len++;
	return len;
}

char *strchr(const char *src, int ch)
{
	if(!src || !src[0])
		return NULL;
	while(*src != ch)
		if(!*src++)
			return NULL;
	return (char*)src;
}

int strcmp(const char *s1, const char *s2)
{
	unsigned s1l = strlen(s1);
	//unsigned s2l = strlen(s2);
	unsigned i = 0;
	unsigned r = 0;
	while ((s1[i] != NULL && s2[i] != NULL) && !(r = s1[i] - s2[i]))
		++i;
	if (s1l - 1 > i) { return r; } else { return 0; }
}

char *strcat(char *dest, const char *src)
{
	if(!dest || !src)
		return NULL;
	
	// Save a copy of the pointer
	char *dest_save = dest;
	// Go to the end of the array
	dest += strlen(dest);
	
	// Copy src to the end of dest
	while((*dest++ = *src++))
		continue;
	
	// Return the beginning of the array.
	return (dest = dest_save);
}

char *strncat(char *restrict dest, const char *restrict src, size_t n)
{
	if(!dest || !src)
		return NULL;
	
	char *dest_save = dest;
	dest += n;
	
	while((*dest++ = *src++))
		continue;
	
	return (dest = dest_save);
}

char *strrev(char *_str)
{
	/*size_t i = 0, j = strlen(_str)-1;
	char tmp;
	while (i < j)
	{
		tmp = _str[i];
		_str[i] = _str[j];
		_str[j] = tmp;
		i++;
		j--;
	}
	return _str;*/
	// I'm cheap.
	return (char*)memrev((void*)_str, (const void*)_str, strlen(_str));
}

char *strnrev(char *_str, size_t len)
{
/*	size_t i = 0, j = len-1;
	char tmp;
	while (i < j)
	{
		tmp = _str[i];
		_str[i] = _str[j];
		_str[j] = tmp;
		i++;
		j--;
	}
	return _str;*/
	// Still cheap.
	return (char*)memrev((void*)_str, (const void*)_str, len);
}

char *index(const char *s, int c)
{
	return NULL; // TODO: this
}

char *rindex(const char *s, int c)
{
	return NULL; // TODO: this.
}

char *strstr(const char *haystack, const char *needle)
{
	return NULL; // TODO: this
}

char *strncpy(char *dest, const char *src, size_t n)
{
	return NULL; // TODO: this.
}

char *strtoupper(char *dest, const char *src)
{
	while (*src)
		*dest++ = toupper(*src++);
	return dest;
}

char *strtolower(char *dest, const char *src)
{
	while(*src)
		*dest++ = tolower(*src++);
	return dest;
}

int strcasecmp(const char *s1, const char *s2)
{
	// Make a better version of this.
	char str[(1<<16)];
	return strcmp(strtolower(str, s1), strtolower(str, s2));
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Mem* functions /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void *memset(void *s, int c, size_t n)
{
	while(n-- > 0)
		*(char*)s++ = c;
	return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	while(n-- > 0)
		*(char*)dest++ = *(char*)src++;
	return dest;
}

int memcmp(const void *cmp1, const void *cmp2, size_t n)
{
	unsigned char *p = (unsigned char*)cmp1;
	unsigned char *r = (unsigned char*)cmp2;
	while(n-- > 0 && *p++ && *r++)
		if(*r != *p)
			return (int)(*p - *r);
	return 0;
}

void *memrev(void *dest, const void *src, size_t n)
{
	// Iterators, s is beginning, e is end.
	unsigned char *s = (unsigned char*)dest, *e = ((unsigned char*)dest) + n - 1;

	// Copy to out buffer for our work
	memcpy(dest, src, n);

	// Iterate and reverse copy the bytes
	for(; s < e; ++s, --e)
	{
		unsigned char t = *s;
		*s = *e;
		*e = t;
	}

	// Return provided buffer
	return dest;
}

