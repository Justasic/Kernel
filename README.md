Bnyeh Kernel
=============

I'm completely insane now, I am writing my own kernel to learn how low-level hardware programming works.

Goal
----

The goal of the kernel (for the moment) is to get a user-space setup with a small N64 emulator
and having enough APIs to run and play the games. This is both to teach me how to write a use
for a kernel as well as writing an emulator.

Why?
----

I need/want to learn low-level hardware development so I can possibly contribute to the
linux kernel. I also want to try and use C/assembly in a kernel space with full control
over the hardware and how it operates.

*AND BECAUSE I AM INSANE OF COURSE!*


Compiling
=========

CMake will automatically try and use Clang over GCC because Clang is easier to
debug with and produces binaries faster than GCC (plus it's my favorite compiler).

If you don't have clang then CMake *should* default to the system compiler but
if it doesn't then please file a bug.

To compile with gcc:
-------------------

1. run `tools/mkcrosscompiler_gcc.sh`
2. `mkdir build`
3. `cd build`
4. `cmake -DUSE_GCC_TOOLCHAIN:BOOLEAN=TRUE ..`
5. `make`
6. `make iso`

This will build the kernel and make an iso file located as Kernel.bin and Kernel.iso in the root of the build directory.

Note: You can change the build directory to be anywhere on the computer, see CMake's website for that.


To compile with clang:
----------------------

Install clang from your distro's package manager

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`
5. `make iso`

This will build the kernel with clang. CMake will automatically pick the right
settings for cross-compilation to i386 processors.
