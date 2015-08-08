#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_FLAG_MEM 0x001
#define MULTIBOOT_FLAG_DEVICE 0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS 0x008
#define MULTIBOOT_FLAG_AOUT 0x010
#define MULTIBOOT_FLAG_ELF 0x020
#define MULTIBOOT_FLAG_MMAP 0x040
#define MULTIBOOT_FLAG_CONFIG 0x080
#define MULTIBOOT_FLAG_LOADER 0x100
#define MULTIBOOT_FLAG_APM 0x200
#define MULTIBOOT_FLAG_VBE 0x400

typedef struct multiboot_memory_map {
	uint32_t size;
	uint32_t base_addr_low,base_addr_high;
	uint64_t length_low,length_high;
	uint64_t type;
} multiboot_memory_map_t;

struct multiboot
{
	uint32_t flags;            // multiboot info version number
	uint32_t mem_lower;        // available memory from the bios
	uint32_t mem_upper;
	uint32_t boot_device;      // "root" partition
	uint32_t cmdline;          // kernel command line
	uint32_t mods_count;       // Boot-module list
	uint32_t mods_addr;
	uint32_t num;              // Secion header table for ELF
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
	uint32_t mmap_length;      // Memory Mapping buffer
	uint32_t mmap_addr;
	uint32_t drives_length;    // Drive info buffer
	uint32_t drives_addr;
	uint32_t config_table;     // ROM configuration table
	uint32_t boot_loader_name; // Bootloader name
	uint32_t apm_table;        // APM table
	uint32_t vbe_control_info; // Video-related stuff.
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
} __attribute__((packed));

typedef struct multiboot multiboot_t;

#endif