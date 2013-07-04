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

// mem* funcs
extern int memcmp(const void *cmp1, const void *cmp2, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *s, int c, size_t n);

// extended libc funcs
extern char *strtoupper(char *dest, const char *src);
extern char *strtolower(char *dest, const char *src);

#endif
