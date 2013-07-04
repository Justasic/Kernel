// Video memory pointer - always constant
unsigned short *vidmem = (unsigned short *) 0xB8000;

void
vidmemwrite(const char* str) {
  unsigned i;
  for (i = 0; str[i] != NULL; ++i) vidmem[i] = (unsigned char) *str | 0x0700;
}
