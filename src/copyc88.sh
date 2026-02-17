#!/usr/bin/env bash
#
# copyc866sh - copy C88 sources to ELKS for self-compilation and building libc
#   ELKS Usage: cd test; make
#
set -e

# NOTE: Change this to ELKS repo top directory
TOPDIR=/Users/greg/net/elks-gh

DEST=$TOPDIR/elkscmd/rootfs_template/root/test

rm -rf $DEST
mkdir -p $DEST
mkdir -p $DEST/C
mkdir -p $DEST/GEN
mkdir -p $DEST/ASM
mkdir -p $DEST/NBIND
mkdir -p $DEST/LIB88
mkdir -p $DEST/OTHER
mkdir -p $DEST/libc
mkdir -p $DEST/libc/system
mkdir -p $DEST/libc/builtin
mkdir -p $DEST/libc/misc

cp -p Makefile.elks             $DEST/Makefile

cp -p C/C88.C                   $DEST/C/C88.c
cp -p C/C2.C                    $DEST/C/C2.c
cp -p C/C3.C                    $DEST/C/C3.c
cp -p C/C4.C                    $DEST/C/C4.c
cp -p C/C5.C                    $DEST/C/C5.c
cp -p C/C6.C                    $DEST/C/C6.c
cp -p C/C7.C                    $DEST/C/C7.c
cp -p C/*.H                     $DEST/C
cp -p C/Makefile.elks           $DEST/C/Makefile

cp -p GEN/GEN.C                 $DEST/GEN/GEN.c
cp -p GEN/GEN2.C                $DEST/GEN/GEN2.c
cp -p GEN/GEN3.C                $DEST/GEN/GEN3.c
cp -p GEN/GEN4.C                $DEST/GEN/GEN4.c
cp -p GEN/GEN5.C                $DEST/GEN/GEN5.c
cp -p GEN/GEN6.C                $DEST/GEN/GEN6.c
cp -p GEN/*.H                   $DEST/GEN
cp -p GEN/Makefile.elks         $DEST/GEN/Makefile

cp -p ASM/ASM88.C               $DEST/ASM/ASM88.c
cp -p ASM/ASM2.C                $DEST/ASM/ASM2.c
cp -p ASM/ASM3.C                $DEST/ASM/ASM3.c
cp -p ASM/ASM4.C                $DEST/ASM/ASM4.c
cp -p ASM/ASM5.C                $DEST/ASM/ASM5.c
cp -p ASM/*.H                   $DEST/ASM
cp -p ASM/Makefile.elks         $DEST/ASM/Makefile

cp -p NBIND/NBIND.C             $DEST/NBIND/NBIND.c
cp -p NBIND/*.H                 $DEST/NBIND
cp -p NBIND/Makefile.elks       $DEST/NBIND/Makefile

cp -p LIB88/LIB88.C             $DEST/LIB88/LIB88.c
cp -p LIB88/*.H                 $DEST/LIB88
cp -p LIB88/Makefile.elks       $DEST/LIB88/Makefile

cp -p OTHER/DUMPO.C             $DEST/OTHER/DUMPO.c
cp -p OTHER/TOOBJ.C             $DEST/OTHER/TOOBJ.c
cp -p OTHER/*.H                 $DEST/OTHER
cp -p OTHER/Makefile.elks       $DEST/OTHER/Makefile

cp -p libc/Makefile             $DEST/libc
cp -p libc/system/*             $DEST/libc/system
cp -p libc/builtin/*            $DEST/libc/builtin
cp -p libc/misc/*               $DEST/libc/misc

#cp -p libc/libc86.a                         $DEST/lib

#cp -p libc/include/alloca.h                 $DEST/include
#cp -p libc/include/ctype.h                  $DEST/include
#cp -p libc/include/errno.h                  $DEST/include
#cp -p libc/include/fcntl.h                  $DEST/include
#cp -p libc/include/features.h               $DEST/include
#cp -p libc/include/limits.h                 $DEST/include
#cp -p libc/include/malloc.h                 $DEST/include
#cp -p libc/include/signal.h                 $DEST/include
#cp -p libc/include/stdint.h                 $DEST/include
#cp -p libc/include/stdlib.h                 $DEST/include
#cp -p libc/include/stdio.h                  $DEST/include
#cp -p libc/include/string.h                 $DEST/include
#cp -p libc/include/termios.h                $DEST/include
#cp -p libc/include/time.h                   $DEST/include
#cp -p libc/include/unistd.h                 $DEST/include
#cp -p libc/include/sys/cdefs.h              $DEST/include/sys
#cp -p libc/include/sys/ioctl.h              $DEST/include/sys
#cp -p libc/include/sys/select.h             $DEST/include/sys
#cp -p libc/include/sys/stat.h               $DEST/include/sys
#cp -p libc/include/sys/types.h              $DEST/include/sys
#cp -p libc/include/c86/*.h                  $DEST/include/c86

echo "Files copied"
