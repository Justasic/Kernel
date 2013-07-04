# Make a floppy image
add_custom_target(
	floppy
	DEPENDS
	Kernel.bin
	floppy_internal1
	floppy_internal2
	floppy_internal3
	floppy_internal4
	floppy_internal5
	floppy_internal6
	#floppy_internal7
)
add_custom_target(floppy_internal1 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_custom_target(floppy_internal2 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_custom_target(floppy_internal3 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_custom_target(floppy_internal4 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_custom_target(floppy_internal5 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_custom_target(floppy_internal6 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
#add_custom_target(floppy_internal7 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_command(
	COMMAND sudo
	ARGS /sbin/losetup /dev/loop0 etc/floppy.img
	#TARGET floppy
	OUTPUT floppy_internal1
)

add_custom_command(
	COMMAND sudo
	ARGS mount /dev/loop0 /mnt2
	OUTPUT floppy_internal2
	DEPENDS floppy_internal1
)

add_custom_command(
	COMMAND sudo
	ARGS cp ${CMAKE_BINARY_DIR}/kernel.bin /mnt2/kernel
	OUTPUT floppy_internal3
	DEPENDS floppy_internal2
)

# add_custom_command(
# 	COMMAND sudo
# 	ARGS cp ${CMAKE_BINARY_DIR}/initrd.img /mnt2/initrd
# 	TARGET floppy
# 	OUTPUT /mnt2/initrd
# 	DEPENDS /mnt2/kernel
# )

add_custom_command(
	COMMAND sudo
	ARGS umount /dev/loop0
	OUTPUT floppy_internal4
	DEPENDS floppy_internal3
)

add_custom_command(
	COMMAND sudo
	ARGS /sbin/losetup -d /dev/loop0
	OUTPUT floppy_internal5
	DEPENDS floppy_internal4
)

add_custom_command(
	COMMAND cp 
	ARGS etc/floppy.img ${CMAKE_BINARY_DIR}/floppy.img
	OUTPUT floppy_internal6
	DEPENDS floppy_internal5
)
