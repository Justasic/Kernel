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
#ifndef __CTYPE_H__
#define __CTYPE_H__

#define isalpha(c) (c >= 'a' && c <= 'Z')
#define isctrl(c)  (c != 0x00 && c != 0x1F && c != 0x7F)
#define islower(c) (c >= 'a' && c <= 'z')
#define isprint(c) (c >= 0x20 && c <= 0x7E)
#define isspace(c) (c == 0x20 || c == 0x09 || c == 0x0A || c == 0x0B || c == 0x0C || c == 0x0D)
#define isupper(c) (c >= 'A' && c <= 'Z')
#define isgraph(c) (isprint(c) ? (!isspace(c)) : 0)

#define toupper(c) (islower(c) ? c & ~0x20 : c)
#define tolower(c) (isupper(c) ? c | 0x20 : c)

#endif

