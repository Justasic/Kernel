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
#ifndef __STRING_H__
#define __STRING_H__
#include "stdint.h"

extern size_t strlen(const char*);
extern char *strchr(const char *s, int c);
extern int strcmp(const char *s1, const char *s2);
extern int strcasecmp(const char *s1, const char *s2);
extern char *strcat(char *dest, const char *src);
extern char *index(const char *s, int c);
extern char *rindex(const char *s, int c);
extern char *strstr(const char *haystack, const char *needle);
extern char *strncpy(char *dest, const char *src, size_t n);
extern char *strrev(char *_str);
extern char *strnrev(char *_str, size_t len);

// mem* funcs
extern int memcmp(const void *cmp1, const void *cmp2, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *s, int c, size_t n);

// extended libc funcs
extern char *strtoupper(char *dest, const char *src);
extern char *strtolower(char *dest, const char *src);

#endif
