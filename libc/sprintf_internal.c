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
#include "string.h"

/* flags used in processing format string */
#define PR_LJ   0x01    /* left justify */
#define PR_CA   0x02    /* use A-F instead of a-f for hex */
#define PR_SG   0x04    /* signed numeric conversion (%d vs. %u) */
#define PR_32   0x08    /* long (32-bit) numeric conversion */
#define PR_16   0x10    /* short (16-bit) numeric conversion */
#define PR_WS   0x20    /* PR_SG set and num was < 0 */
#define PR_LZ   0x40    /* pad left with '0' instead of ' ' */
#define PR_FP   0x80    /* pointers are far */

/* largest number handled is 2^32-1, lowest radix handled is 8.
 2^32-1 in base 8 has 11 digits (add 5 for trailing NUL and for slop) */
#define PR_BUFLEN       16


int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
	// The source for this was taken from Brain's do_printf function.
	// the original source can be found at:
	// https://brainbox.cc/repository/trunk/sixty-four/kernel64/src/printf.c
	unsigned flags, actual_wd, count, given_wd;
	uint8_t *where, buf[PR_BUFLEN];
	uint8_t state, radix;
	long num;

	state = flags = count = given_wd = 0;
	/* begin scanning format specifier list */
	for(; *fmt && count < size; fmt++)
	{
		switch(state)
		{
		/* STATE 0: AWAITING % */
		case 0:
			if(*fmt != '%') /* not %... */
			{
				*str++ = *fmt; /* ...just echo it */
				//fn(*fmt, &ptr); 
				count++;
				break;
			}
			/* found %, get next char and advance state to check if next char is a flag */
			state++;
			fmt++;
			/* FALL THROUGH */
			/* STATE 1: AWAITING FLAGS (%-0) */
		case 1:
			if(*fmt == '%') /* %% */
			{
				*str++ = *fmt;
// 				fn(*fmt, &ptr);
				count++;
				state = flags = given_wd = 0;
				break;
			}
			if(*fmt == '-')
			{
				if(flags & PR_LJ)/* %-- is illegal */
					state = flags = given_wd = 0;
				else
					flags |= PR_LJ;
				break;
			}
			/* not a flag char: advance state to check if it's field width */
			state++;
			/* check now for '%0...' */
			if(*fmt == '0')
			{
				flags |= PR_LZ;
				fmt++;
			}
			/* FALL THROUGH */
			/* STATE 2: AWAITING (NUMERIC) FIELD WIDTH */
		case 2:
			if(*fmt >= '0' && *fmt <= '9')
			{
				given_wd = 10 * given_wd +
					(*fmt - '0');
				break;
			}
			/* not field width: advance state to check if it's a modifier */
			state++;
			/* FALL THROUGH */
			/* STATE 3: AWAITING MODIFIER CHARS (FNlh) */
		case 3:
			if(*fmt == 'F')
			{
				flags |= PR_FP;
				break;
			}
			if(*fmt == 'N')
				break;
			if(*fmt == 'l')
			{
				flags |= PR_32;
				break;
			}
			if(*fmt == 'h')
			{
				flags |= PR_16;
				break;
			}
			/* not modifier: advance state to check if it's a conversion char */
			state++;
			/* FALL THROUGH */
			/* STATE 4: AWAITING CONVERSION CHARS (Xxpndiuocs) */
		case 4:
			where = buf + PR_BUFLEN - 1;
			*where = '\0';
			switch(*fmt)
			{
			case 'X':
				flags |= PR_CA;
				/* FALL THROUGH */
				/* xxx - far pointers (%Fp, %Fn) not yet supported */
			case 'x':
			case 'p':
			case 'n':
				radix = 16;
				goto DO_NUM;
			case 'd':
			case 'i':
				flags |= PR_SG;
				/* FALL THROUGH */
			case 'u':
				radix = 10;
				goto DO_NUM;
			case 'b':
				radix = 2;
				goto DO_NUM;
			case 'o':
				radix = 8;
				/* load the value to be printed. l=long=32 bits: */
			DO_NUM:
				if(flags & PR_32)
					num = va_arg(ap, unsigned long);
				/* h=short=16 bits (signed or unsigned) */
				else if(flags & PR_16)
				{
					if(flags & PR_SG)
						num = va_arg(ap, int);
					else
						num = va_arg(ap, unsigned int);
				}
				/* no h nor l: sizeof(int) bits (signed or unsigned) */
				else
				{
					if(flags & PR_SG)
						num = va_arg(ap, int);
					else
						num = va_arg(ap, unsigned int);
				}
				/* take care of sign */
				if(flags & PR_SG)
				{
					if(num < 0)
					{
						flags |= PR_WS;
						num = -num;
					}
				}
				/* convert binary to octal/decimal/hex ASCII */
				do
				{
					unsigned long temp = (unsigned long)num % radix;
					where--;
					if(temp < 10)
						*where = temp + '0';
					else if(flags & PR_CA)
						*where = temp - 10 + 'A';
					else
						*where = temp - 10 + 'a';
					num = (unsigned long)num / radix;
				}
				while(num != 0);
				goto EMIT;
			case 'c':
				/* disallow pad-left-with-zeroes for %c */
				flags &= ~PR_LZ;
				where--;
				*where = (unsigned char)va_arg(ap, int);
				actual_wd = 1;
				goto EMIT2;
			case 'y':
				num = va_arg(ap, unsigned long);
				where = (num % 2) ? "true" : "false";
				goto EMIT;
			case 's':
				/* disallow pad-left-with-zeroes for %s */
				flags &= ~PR_LZ;
				where = va_arg(ap, unsigned char *);
			EMIT:
				actual_wd = strlen((const char*)where);
				if(flags & PR_WS)
					actual_wd++;
				/* if we pad left with ZEROES, do the sign now */
				if((flags & (PR_WS | PR_LZ)) ==
					(PR_WS | PR_LZ))
				{
// 					fn('-', &ptr);
					*str++ = '-';
					count++;
				}
				/* pad on left with spaces or zeroes (for right justify) */
			EMIT2:
				if((flags & PR_LJ) == 0)
				{
					while(given_wd > actual_wd)
					{
// 						fn(flags & PR_LZ ? '0' : ' ', &ptr);
						*str++ = (flags & PR_LZ ? '0' : ' ');
						count++;
						given_wd--;
					}
				}
				/* if we pad left with SPACES, do the sign now */
				if((flags & (PR_WS | PR_LZ)) == PR_WS)
				{
// 					fn('-', &ptr);
					*str++ = '-';
					count++;
				}
				/* emit string/char/converted number */
				while(*where != '\0')
				{
// 					fn(*where++, &ptr);
					*str++ = *where++;
					count++;
				}
				/* pad on right with spaces (for left justify) */
				if(given_wd < actual_wd)
					given_wd = 0;
				else given_wd -= actual_wd;
					for(; given_wd; given_wd--)
					{
// 						fn(' ', &ptr);
						*str++ = ' ';
						count++;
					}
				break;
			default:
				break;
			}
		default:
			state = flags = given_wd = 0;
			break;
		}
	}
	
	return count;
}
