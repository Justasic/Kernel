cmake_minimum_required(VERSION 2.8)

# Force external build
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)
	message(FATAL_ERROR "You can not use CMake to build from the root of it's source tree! Remove the CMakeCache.txt file from this directory, then create a separate directory (either below this directory or elsewhere), and then re-run CMake from there.")
endif(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)

# Cmake module path (if exists)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(profile_detection)

# http://wiki.osdev.org/CMake_Build_System
# This was very helpful in this build system
LOAD_PROFILE("i386" "pc") # Generic for now

# Get all the files needed
file(GLOB KERN_TTY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "kern/tty/*.c")
file(GLOB KERN_LIB_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "kern/lib/*.c")
set(KERN_SOURCES ${KERN_TTY_SOURCES} ${KERN_LIB_SOURCES})
# file(GLOB KERN_TTY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "kern/tty/*.c")
file(GLOB LIBC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "libc/*.c")

# Various options we need
option(USE_GCC_TOOLCHAIN "Use the GCC toolchain from tools/mkcrosscompiler_gcc.sh" OFF)
option(USE_LLVM_TOOLCHAIN "Use the clang toolchain from tools/mkcrosscompiler_clang.sh" ON)

if(${USE_GCC_TOOLCHAIN} STREQUAL "OFF" AND ${USE_LLVM_TOOLCHAIN} STREQUAL "OFF")
	message(STATUS "\nWARNING: Unspecified toolchain can result in bad cross-compiles, any bugs as a result of this will be marked as wont-fix if reported!!\n
	Valid toolchains are:\n-DUSE_GCC_TOOLCHAIN:BOOLEAN=TRUE -  Use GCC\n-DUSE_LLVM_TOOLCHAIN:BOOLEAN=TRUE - Use Clang")
endif(${USE_GCC_TOOLCHAIN} STREQUAL "OFF" AND ${USE_LLVM_TOOLCHAIN} STREQUAL "OFF")

# these are any additional flags which should be added to the base compiles. Things like 32 bit or elf binaries
set(CFLAGS "${ISA_C_FLAGS} -m32 -std=c11")
set(LINKFLAGS "-melf_i386 ${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS}")
set(ASM_FLAGS "${ISA_ASM_FLAGS}")

find_program(CLANG clang REQUIRED)
find_program(CLANGXX clang++ REQUIRED)

set(CMAKE_C_COMPILER ${CLANG})
set(CMAKE_CXX_COMPILER ${CLANGXX})


# Finally name our project now that we have picked our compiler.
project(Kernel C)
enable_language(ASM-ATT)

# If we're using clang..
if(${USE_LLVM_TOOLCHAIN} OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
	
	SET (BASE_CFLAGS "${CFLAGS} -ffreestanding -fno-builtin -nostdlib -nostdinc -finline-functions -nostdinc -fno-stack-protector -fomit-frame-pointer -nostdlib")
	
	SET (LINKFLAGS "-Wl,--script=${PLATFORM_LAYOUT} -Wl,--omagic ${LINKFLAGS}")
	
	SET (CMAKE_C_FLAGS                "-Wall -g ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_DEBUG          "-g ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g ${BASE_CFLAGS}")

	SET (CMAKE_CXX_FLAGS                "-Wall ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_DEBUG          "-g ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${BASE_CFLAGS}")
	
	SET (CMAKE_ASM_FLAGS            "-Werror -felf")
	
endif(${USE_LLVM_TOOLCHAIN} OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")


# If we're using our custom gcc from /usr/local/cross/bin
if(${USE_GCC_TOOLCHAIN} OR ${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
	
	SET (BASE_CFLAGS "${CFLAGS} -ffreestanding -fno-builtin -nostdlib -nostdinc -fstrength-reduce -fno-builtin-time -fno-builtin-puts -fno-builtin-printf -fno-builtin-function -finline-functions -nostdinc -fno-stack-protector -fomit-frame-pointer -nostdlib")
	
	SET (LINKFLAGS "-T ${PLATFORM_LAYOUT} -N ${LINKFLAGS}")
	
	SET (CMAKE_C_FLAGS                "-Wall -g ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_DEBUG          "-g ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g ${BASE_CFLAGS}")

	SET (CMAKE_CXX_FLAGS                "-Wall ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_DEBUG          "-g ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${BASE_CFLAGS}")
	
	SET (CMAKE_ASM_FLAGS            "-Werror -felf")

	find_program(GCC_AR i586-elf-ar REQUIRED PATHS /usr/local/cross/bin)
	find_program(GCC_LD i586-elf-ld REQUIRED PATHS /usr/local/cross/bin)
	find_program(GCC_NM i586-elf-nm REQUIRED PATHS /usr/local/cross/bin)
	find_program(GCC_OBJDUMP i586-elf-objdump REQUIRED PATHS /usr/local/cross/bin)
	find_program(GCC_RANLIB i586-elf-ranlib REQUIRED PATHS /usr/local/cross/bin)
	find_program(GCC i586-elf-gcc REQUIRED PATHS /usr/local/cross/bin)
	find_program(GPP i586-elf-g++ REQUIRED PATHS /usr/local/cross/bin)
	message(STATUS "GCC-ar: ${GCC_AR}")
	message(STATUS "GCC-ld: ${GCC_LD}")
	message(STATUS "GCC-nm: ${GCC_NM}")
	message(STATUS "GCC-objdump: ${GCC_OBJDUMP}")
	message(STATUS "GCC-ranlib: ${GCC_RANLIB}")
	message(STATUS "GCC: ${GCC}")
	message(STATUS "G++: ${GPP}")

	SET (CMAKE_AR      "${GCC_AR}")
	SET (CMAKE_LINKER  "${GCC_LD}")
	SET (CMAKE_NM      "${GCC_NM}")
	SET (CMAKE_OBJDUMP "${GCC_OBJDUMP}")
	SET (CMAKE_RANLIB  "${GCC_RANLIB}")
	SET (CMAKE_CXX_COMPILER "${GPP}")
	SET (CMAKE_C_COMPILER "${GCC}")
	
endif(${USE_GCC_TOOLCHAIN} OR ${CMAKE_C_COMPILER_ID} STREQUAL "GNU")

# include cmake stuff for checking various system specific defines
# include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
# include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)
include(TestBigEndian)
# include(TargetArch)

# Check type sizes since we need them
# check_type_size(int8_t INT8_T)
# check_type_size(uint8_t UINT8_T)
# check_type_size(int16_t INT16_T)
# check_type_size(uint16_t UINT16_T)
# check_type_size(int32_t INT32_T)
# check_type_size(uint32_t UINT32_T)
# check_type_size(int64_t INT64_T)
# check_type_size(uint64_t UINT64_T)

include_directories(
	${CMAKE_SOURCE_DIR}/libc
	${CMAKE_SOURCE_DIR}/kern
)

# Concatenate and sort
set(SOURCE_FILES ${LIBC_SOURCES} ${KERN_SOURCES} ${ISA_SRCS} ${PLATFORM_SRCS})
list(SORT SOURCE_FILES)

# Find perl - for run-cc.pl to colorize errors/warnings during compile
find_program(PERL perl)

# Compile -  We need custom options for building a kernel

if(PERL AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
	# C++ compiles
	set(CMAKE_CXX_COMPILE_OBJECT "${PERL} ${CMAKE_SOURCE_DIR}/run-cc.pl -q ${CMAKE_CXX_COMPILE_OBJECT}")
	set(CMAKE_CXX_LINK_EXECUTABLE "${PERL} ${CMAKE_SOURCE_DIR}/run-cc.pl -q ${CMAKE_CXX_LINK_EXECUTABLE}")
	set(CMAKE_CXX_CREATE_SHARED_MODULE "${PERL} ${CMAKE_SOURCE_DIR}/run-cc.pl -q ${CMAKE_CXX_CREATE_SHARED_MODULE}")
	# C compiles
	set(CMAKE_C_COMPILE_OBJECT "${PERL} ${CMAKE_SOURCE_DIR}/run-cc.pl -q ${CMAKE_C_COMPILE_OBJECT}")
	set(CMAKE_C_LINK_EXECUTABLE "${PERL} ${CMAKE_SOURCE_DIR}/run-cc.pl -q ${CMAKE_C_LINK_EXECUTABLE}")
	set(CMAKE_C_CREATE_SHARED_MODULE "${PERL} ${CMAKE_SOURCE_DIR}/run-cc.pl -q ${CMAKE_C_CREATE_SHARED_MODULE}")
	# Assembly compiles
	set(CMAKE_ASM_COMPILE_OBJECT "${PERL} ${CMAKE_SOURCE_DIR}/run-cc.pl -q <CMAKE_ASM-ATT_COMPILER> ${ASM_FLAGS} -o <OBJECT> <SOURCE>")
else(PERL AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
	# We're not doing colored builds or we're clang.
	# Assembly compiles
	SET(CMAKE_ASM-ATT_COMPILE_OBJECT  "<CMAKE_ASM-ATT_COMPILER> ${ASM_FLAGS} -o <OBJECT> <SOURCE>")
endif(PERL AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")

add_executable(${PROJECT_NAME}.bin ${SOURCE_FILES})
set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE C COMPILE_FLAGS "")
set_target_properties(${PROJECT_NAME}.bin PROPERTIES LINKER_LANGUAGE C PREFIX "" SUFFIX "" LINK_FLAGS "${LINKFLAGS}")

include(make_iso)
