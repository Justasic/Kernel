#include "string.h"
#include "stddef.h"
#include "ctype.h"

size_t strlen(const char *str)
{
	size_t len = 0;
	while(*str++)
		len++;
	return len;
}

char *strchr(const char *s, int c)
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
	unsigned s2l = strlen(s2);
	unsigned i = 0;
	unsigned r = 0;
	while ((s1[i] != NULL && s2[i] != NULL) && !(r = s1[i] - s2[i]))
		++i;
	if (s1l - 1 > i) { return r; } else { return 0; }
}

char *strcat(char *dest, const char *src)
{
	if(!dest || !dest[0])
		return NULL;
	while(*src)
		*dest++ = *src++;
	return dest;
}

char *strncat(char *restrict dest, const char *restrict src, size_t n)
{
	if(!dest || !dest[0])
		return NULL;
	while(n-- > 0)
		*dest++ = *src++;
	return dest;
}

char *index(const char *s, int c)
{
	return s; // TODO: this
}

char *rindex(const char *s, int c)
{
	return s; // TODO: this.
}

char *strstr(const char *haystack, const char *needle)
{
	return haystack; // TODO: this
}

char *strncpy(char *dest, const char *src, size_t n)
{
	return src; // TODO: this.
}

char *strtoupper(char *dest, const char *src)
{
	while (*src)
		*dest++ = toupper(*str++);
	return dest;
}

char *strtolower(char *dest, const char *src)
{
	while(*src)
		*dest++ = tolower(*str++);
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

