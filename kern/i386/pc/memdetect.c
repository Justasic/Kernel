

#include "i386/pc/multiboot.h"

#if 0
void DetectSystemMemory(multiboot_t *mboot)
{
	multiboot_memory_map_t *m = mboot->mmap_addr;
	while (m < mboot->mmap_addr + mboot->mmap_length)
		m = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
}
#endif