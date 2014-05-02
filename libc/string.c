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

// TODO: this is temporary to clear away some of the warnings
#warning "Developer notice: This file is incomplete."
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wreturn-type"

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Mem* functions /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void *memset(void *s, int c, size_t n)
{
	while (n-- > 0)
		*(char*)s++ = c;
	return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	while (n-- > 0)
		*(char*)dest++ = *(char*)src++;
	return dest;
}

int memcmp(const void *cmp1, const void *cmp2, size_t n)
{
	unsigned char *p = (unsigned char*)cmp1;
	unsigned char *r = (unsigned char*)cmp2;
	while (n-- > 0 && *p++ && *r++)
	{
		if (*r != *p)
			return (int)(*p - *r);
	}
	return 0;
}

void *memrev(void *dest, const void *src, size_t n)
{
	// Iterators, s is beginning, e is end.
	unsigned char *s = (unsigned char*)dest, *e = ((unsigned char*)dest) + n - 1;
	
	// Copy to out buffer for our work
	memcpy(dest, src, n);
	
	// Iterate and reverse copy the bytes
	for (; s < e; ++s, --e)
	{
		unsigned char t = *s;
		*s = *e;
		*e = t;
	}
	
	// Return provided buffer
	return dest;
}

size_t strlen(const char *str)
{
	size_t len = 0;
	while (*str++)
		len++;
	return len;
}

char *strchr(const char *src, int ch)
{
	if (!src || !src[0])
		return NULL;
	
	while (*src != ch)
		if (!*src++)
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
	
	if (s1l - 1 > i)
		return r; 
	else
		return 0;
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
	while ((*dest++ = *src++))
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
	
	while ((*dest++ = *src++))
		continue;
	
	return (dest = dest_save);
}

char *strrev(char *_str)
{
	// I'm cheap.
	return (char*)memrev((void*)_str, (const void*)_str, strlen(_str));
}

char *strnrev(char *_str, size_t len)
{
	// Still cheap.
	return (char*)memrev((void*)_str, (const void*)_str, len);
}

char *index(const char *s, int c)
{
	if (!s)
		return NULL;

	while (*s++)
	{
		if (*s == c) // WARNING: XXX: ICKY!!
			return (char *)s;
	}
	return NULL;
}

char *rindex(const char *s, int c)
{
	// go to end of string.
	// WARNING: XXX: ICKY!!
	char *str = ((char*)s + strlen(s));
	
	while (*str--)
	{
		if (*str == c)
			return str;
	}
	
	return NULL;
}

char *strstr(const char *haystack, const char *needle)
{
	return NULL; // TODO: this
}

char *strncpy(char *dest, const char *src, size_t n)
{
	if (!dest || !src || !n)
		return NULL;
	// I am really cheap.
	memcpy(dest, src, n);
	return dest;
}

char *strcpy(char *dest, const char *src)
{
	if (!dest || !src)
		return NULL;
	// really, really cheap.
	memcpy(dest, src, strlen(src));
	return dest;
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
	// TODO: Make a better version of this.
	char str[(1<<16)];
	return strcmp(strtolower(str, s1), strtolower(str, s2));
}

char *strerror(int errnum)
{
	return NULL; // TODO: this.
}

#pragma clang diagnostic pop
