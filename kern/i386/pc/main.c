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
		vidmem[i] = (unsigned char) *ch | 0x0700;

	for(;;) // Idle loop to stop system.
		// We should tell the CPU to ignore all maskable interrupts
		// and halt instead of an idle loop though..
		;
}

