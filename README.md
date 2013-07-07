Kernel
======

I'm completely insane now, I am writing my own kernel to learn how low-level hardware programming works.



Compiling
=========

To compile with gcc:
-------------------

1. run `tools/mkcrosscompiler_gcc.sh`
2. `mkdir build`
3. `cd build`
4. `cmake -DUSE_GCC_TOOLCHAIN:BOOLEAN=TRUE ..`
5. `make`
6. `make iso`

This will build the kernel and make an iso file located as Kernel.iso in the root of the build directory.

Note: You can change the build directory to be anywhere on the computer, see CMake's website for that.


To compile with clang:
----------------------

[coming soon]