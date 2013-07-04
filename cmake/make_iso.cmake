# Setup a directory for the ISO
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isodir ${CMAKE_BINARY_DIR}/isodir/boot ${CMAKE_BINARY_DIR}/isodir/boot/grub)

# Write the grub menu option
file(WRITE ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg "menuentry \"Justasic Kernel\" {\n	multiboot /boot/Kernel.bin\n}")

find_program(MAKE_RESCUE grub-mkrescue REQUIRED)

add_custom_target(
	iso # Make target
	COMMAND cp ${CMAKE_BINARY_DIR}/Kernel.bin ${CMAKE_BINARY_DIR}/isodir/boot/Kernel.bin
	DEPENDS Kernel.bin
)

add_custom_command(
	COMMAND ${MAKE_RESCUE}
	ARGS -o Kernel.iso ${CMAKE_BINARY_DIR}/isodir 
	TARGET iso
)
