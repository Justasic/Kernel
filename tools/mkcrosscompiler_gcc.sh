#!/bin/bash
# Because I am tired of doing this manually.
#
# Written by Aaron Blakely <aaron@ephasic.org>

if [ `whoami` != "root" ]; then
	echo "Error: Need to run as root."
	exit
fi

GCCURL="http://ftp.gnu.org/gnu/gcc/gcc-4.2.1/gcc-4.2.1.tar.bz2"
BINUTILSURL="http://ftp.gnu.org/gnu/binutils/binutils-2.22.tar.gz"

# By default this will install the cross compiler
# and binutils to /usr/local/cross
#
PREFIX="/usr/local/cross"
TARGET="i586-elf"

# Release teh dragons!
cd /usr/src
mkdir build-binutils build-gcc

echo "Downloading gcc 4.2.1... :)"
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

echo "Your environment should now have a working i586-elf cross compiler tool chain!"
echo 'But first, you need to do this:'
echo '	export PATH=$PATH:/usr/local/cross/bin'
echo " "
echo "Also make sure you have a working nasm assembler."

