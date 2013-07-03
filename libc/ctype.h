#ifndef __CTYPE_H__
#define __CTYPE_H__

#define isalpha(c) (c >= 'a' && c <= 'Z')
#define isctrl(c)  (c != 0x00 && c != 0x1F && c != 0x7F)
#define islower(c) (c >= 'a' && c <= 'z')
#define isprint(c) (c >= 0x20 && c <= 0x7E)
#define isspace(c) (c == 0x20 || c == 0x09 || c == 0x0A || c == 0x0B || c == 0x0C || c == 0x0D)
#define isupper(c) (c >= 'A' && c <= 'Z')
#define isgraph(c) (isprint(c) ? (!isspace(c)) : 0)

#define toupper(c) (islower(c) ? c - ('a' + 'A') : c)
#define tolower(c) (isupper(c) ? c + ('a' - 'A') : c)

#endif

