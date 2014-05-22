/*
 * Copyright (c) 2014, Justin Crawford <Justasic@gmail.com>
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
#include "linux.h"
#include <string.h>
#include "common.h"

// All functions were written based on the following document:
// http://docs.cs.up.ac.za/programming/asm/derick_tut/syscalls.html

// This source file was defined because during the debugging of this
// kernel, there's a lot of times where it will be compiled as ELF.
// This ELF executable is actually understood by linux (as a statically
// compiled binary) and therefore I have defined a few small functions
// to give us just enough to write to the user's terminal and exit.
// 
// Maybe in the future I might make the kernel run as user-mode emulation
// so I can test different functionality without needing to use KVM/QEMU
// but for now, this is all.
//

bool LinuxCheck(void)
{
	uint8_t eax;
	// Call the linux syscal interrupt, if the eax register still equals
	// 20 (the linux syscall) then we're not in linux. Otherwise
	// we are in linux so return true.
	__asm__ __volatile__("int $0x80" : "=a" (eax) : "a" (20));
	if (eax == 20)
		return false;
	return true;
}

void LinuxExit(int code)
{
	// Call the linux system call to exit the application.
	__asm__ __volatile__("int $0x80" :: "a" (1), "b" (code));
}

size_t LinuxWrite(uint32_t fd, const char *str, size_t len)
{
	size_t retlen = -1;
	__asm__ __volatile__("int $0x80" : "=a" (retlen) : "a" (4), "b" (fd), "c" (str), "d" (len));
	return retlen;
}

void LinuxError(const char *err)
{
	LinuxWrite(2, err, strlen(err));
	LinuxExit(1);
}

void LinuxPuts(const char *str)
{
	LinuxWrite(1, str, strlen(str));
}
