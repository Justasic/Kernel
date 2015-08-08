#!/bin/bash
# Because I am tired of doing this manually.
#
# Written by Aaron Blakely <aaron@ephasic.org>

if [ `whoami` != "root" ]; then
	echo "Error: Need to run as root."
	exit
fi

GCC_VER="4.9.0"
BINUTILS_VER="2.24-4"
GCCURL="http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.bz2"
BINUTILSURL="http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.gz"

# By default this will install the cross compiler
# and binutils to /usr/local/cross
#
PREFIX="/usr/local/cross"
TARGET="i586-elf"

# Release teh dragons!
cd /usr/src
mkdir build-binutils build-gcc

echo "Downloading gcc $GCC_VER ... :)"
wget $GCCURL
echo "Done!  Now contacting the mothership..."
sleep 3s
echo "The mothership requires binutils.  Downloading..."
wget $BINUTILSURL

echo "Extracting the DNA from the seeds."
tar xvf `basename $BINUTILSURL`
tar xvjf `basename $GCCURL`

########################################################
# Start the binutils install
########################################################

cd /usr/src/build-binutils
/usr/src/`basename $BINUTILSURL .tar.gz`/configure --target=${TARGET} --prefix=${PREFIX} --disable-nls
make all
make install
echo "Done installing binutils."

#######################################################
# Start the gcc install
#######################################################

cd /usr/src/build-gcc/
export PATH=$PATH:$PREFIX/bin
/usr/src/`basename $GCCURL .tar.bz2`/configure --target=${TARGET} --prefix=${PREFIX} --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make install-gcc
echo "Done installing gcc."

######################################################

echo "Your environment should now have a working $TARGET  cross compiler tool chain!"
echo 'But first, you need to do this:'
echo '	export PATH=$PATH:/usr/local/cross/bin'
echo " "
echo "Also make sure you have a working nasm assembler."

