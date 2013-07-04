#include "stdio.h"

// Video memory pointer - always constant
unsigned short *vidmem = (unsigned short *) 0xB8000;

// As much as this goes against things with me, there is really no reason
// to return an integer.. Maybe in the future we can provide kernel launch
// arguments from the bootloader but for now? void kern_start(void).
void kern_start(void)
{
	char *str = "Hello world!", *ch;
	unsigned i;
	
	for(ch = str, i = 0; *ch; ch++, i++)
		// The constant 0x07000 is the background and foreground color
		// set in the upper bits of the character.
		// 0xFBCC - FB = foreground and Background; CC = ascii character
		vidmem[i] = (unsigned char) *ch | 0x0700; 
	
}

