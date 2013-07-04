cmake_minimum_required(VERSION 2.8)

# Force external build
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)
	message(FATAL_ERROR "You can not use CMake to build from the root of it's source tree! Remove the CMakeCache.txt file from this directory, then create a separate directory (either below this directory or elsewhere), and then re-run CMake from there.")
endif(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)


project(Kernel C)
enable_language(CXX ASM-ATT)

# Cmake module path (if exists)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


# include cmake stuff for checking various system specific defines
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)
include(TestBigEndian)
include(profile_detection)
# include(TargetArch)

# Check type sizes since we need them
check_type_size(int8_t INT8_T)
check_type_size(uint8_t UINT8_T)
check_type_size(int16_t INT16_T)
check_type_size(uint16_t UINT16_T)
check_type_size(int32_t INT32_T)
check_type_size(uint32_t UINT32_T)
check_type_size(int64_t INT64_T)
check_type_size(uint64_t UINT64_T)

include_directories(
	${CMAKE_SOURCE_DIR}/libc
	${CMAKE_SOURCE_DIR}/kern
)

# Get all the files needed
file(GLOB LIBC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "kern/*.c")
file(GLOB KERN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "libc/*.c")

# http://wiki.osdev.org/CMake_Build_System
# This was very helpful in this build system
LOAD_PROFILE("i386" "pc") # Generic for now

# Concatenate and sort
set(SOURCE_FILES ${LIBC_SOURCES} ${KERN_SOURCES} ${ISA_SRCS} ${PLATFORM_SRCS})
list(SORT SOURCE_FILES)

# Various options we need
option(USE_GCC_TOOLCHAIN "Use the GCC toolchain from tools/mkcrosscompiler_gcc.sh" OFF)
option(USE_LLVM_TOOLCHAIN "Use the clang toolchain from tools/mkcrosscompiler_clang.sh" OFF)

if(${USE_GCC_TOOLCHAIN} STREQUAL "OFF" AND ${USE_LLVM_TOOLCHAIN} STREQUAL "OFF")
	message(FATAL_ERROR "\nPlease specify a compiler toolchain or create one for the compiler you're using\n
	Valid toolchains are:\n-DUSE_GCC_TOOLCHAIN:BOOLEAN=TRUE -  Use GCC\n-DUSE_LLVM_TOOLCHAIN:BOOLEAN=TRUE - Use Clang")
endif(${USE_GCC_TOOLCHAIN} STREQUAL "OFF" AND ${USE_LLVM_TOOLCHAIN} STREQUAL "OFF")

# these are any additional flags which should be added to the base compiles. Things like 32 bit or elf binaries
set(CFLAGS "${ISA_C_FLAGS} -T ${PLATFORM_LAYOUT} -N ${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS}")
set(LINKFLAGS "-melf_i386")
set(ASM_FLAGS "${ISA_ASM_FLAGS}")

# This is the base set of compile flags used in any build on any compiler.
SET (BASE_CFLAGS "${CFLAGS} -ffreestanding -fno-builtin -nostdlib -nostdinc -fstrength-reduce -fno-builtin-time -fno-builtin-puts -fno-builtin-printf -fno-builtin-function -finline-functions -nostdinc -fno-stack-protector -fomit-frame-pointer -nostdlib")


# If we're using clang..
if(${USE_LLVM_TOOLCHAIN})
	#SET (CMAKE_C_COMPILER             "/usr/bin/clang")
	SET (CMAKE_C_FLAGS                "-Wall ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_DEBUG          "-g ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g ${BASE_CFLAGS}")

	#SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
	SET (CMAKE_CXX_FLAGS                "-Wall ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_DEBUG          "-g ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${BASE_CFLAGS}")
	
	SET (CMAKE_ASM_FLAGS            "-Werror -felf")

	find_program(LLVM_AR llvm-ar REQUIRED)
	find_program(LLVM_LD llvm-ld REQUIRED)
	find_program(LLVM_NM llvm-nm REQUIRED)
	find_program(LLVM_OBJDUMP llvm-objdump REQUIRED)
	find_program(LLVM_RANLIB llvm-ranlib REQUIRED)
	message(STATUS "llvm-ar: ${LLVM_AR}")
	message(STATUS "llvm-ld: ${LLVM_LD}")
	message(STATUS "llvm-nm: ${LLVM_NM}")
	message(STATUS "llvm-objdump: ${LLVM_OBJDUMP}")
	message(STATUS "llvm-ranlib: ${LLVM_RANLIB}")

	SET (CMAKE_AR      "${LLVM_AR}")
	SET (CMAKE_LINKER  "${LLVM_LD}")
	SET (CMAKE_NM      "${LLVM_NM}")
	SET (CMAKE_OBJDUMP "${LLVM_OBJDUMP}")
	SET (CMAKE_RANLIB  "${LLVM_RANLIB}")
endif(${USE_LLVM_TOOLCHAIN})


# If we're using our custom gcc from /usr/local/cross/bin
if(${USE_GCC_TOOLCHAIN})
	SET (CMAKE_C_COMPILER             "/usr/local/cross/bin/i586-elf-gcc")
	SET (CMAKE_C_FLAGS                "-Wall ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_DEBUG          "-g ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG ${BASE_CFLAGS}")
	SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g ${BASE_CFLAGS}")

	SET (CMAKE_CXX_COMPILER             "/usr/local/cross/bin/i586-elf-g++")
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
	message(STATUS "GCC-ar: ${GCC_AR}")
	message(STATUS "GCC-ld: ${GCC_LD}")
	message(STATUS "GCC-nm: ${GCC_NM}")
	message(STATUS "GCC-objdump: ${GCC_OBJDUMP}")
	message(STATUS "GCC-ranlib: ${GCC_RANLIB}")

	SET (CMAKE_AR      "${GCC_AR}")
	SET (CMAKE_LINKER  "${GCC_LD}")
	SET (CMAKE_NM      "${GCC_NM}")
	SET (CMAKE_OBJDUMP "${GCC_OBJDUMP}")
	SET (CMAKE_RANLIB  "${GCC_RANLIB}")
endif(${USE_GCC_TOOLCHAIN})

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
