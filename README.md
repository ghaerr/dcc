# DeSmet C for ELKS
The 1989 DOS DeSmet C toolchain v3.10h (C88, GEN88, ASM88, LIB88, NBIND, DUMPO and TOOBJ)
ported to [ELKS](https://github.com/ghaerr/elks).

A work in progress. The goal is to produce an ANSI C capable compiler toolchain that can compile itself
on 8086 systems.
This could then be used for creating self-reproducing systems based on the 8086, including
[ELKS](https://github.com/ghaerr/elks) or
[UnixV6-8086](https://github.com/ghaerr/UnixV6-8086).

DCC is now able to compile itself and run under ELKS, but still requires a bit of setup
since 'make' has not yet been ported to DCC. The instructions that follow are currently
a bit complicated but will be automated when possible.

To setup to build DCC on ELKS, first ensure that the separate
[8086-toolchain](https://github.com/ghaerr/8086-toolchain) `make` is prebuilt
and in the ELKS hd32-minix.img:
```
$ cd /path/to/ELKS
$ make
[ensure OpenWatcom v2 installed and WATCOM= set]
$ ./buildext.sh 8086-toolchain
$ cd images
$ make hd32-minix
```
This will produce a 32MB ELKS image with the 8086-toolchain installed. Only the `make` program is currently required, which should now be in /bin.

The next steps will bootstrap the DCC toolchain by compiling on the host system,
after which the DCC source will be copied to the ELKS /root/test directory:
```
$ cd /path/to/ELKS
$ . env.sh  (sets TOPDIR= and PATH= to ia16-elf-gcc toolchain)
$ cd /path/to/DCC
$ make
[edit copyc88.sh and change TOPDIR= to TOPDIR value set above in env.sh)
$ ./copyc88.sh
```
The DCC source should now be in the /root/test template for the next HD32 image
to be built on ELKS. A few final environment variables need to be setup on
the target ELKS /root directory:
```
$ cd $TOPDIR/elkscmd/rootfs_template/root
(add the following lines to new file 'pass1`:
export PATH=/root/bin:/bin
export DSINC=/root/test/libc/include
export DSLIB=/root/test/libc
(add the following lines to new file 'pass2':
export PATH=/root/bin2:/bin
export DSINC=/root/test/libc/include
export DSLIB=/root/test/libc
$ cd image
$ make hd32-minix
```
Now, finally, we're ready to actually build DCC. Boot the hd32-minix.img in the
last step above, and login as root. Then execute the following to bootstrap the
DCC compiler:
```
$ . pass1   (sets build to use /root/bin directory for DCC compiled by ia16-elf-gcc)
$ cd test
$ make      (builds DCC using gcc-built DCC into /bin2)
$           (DCC should now be built in /bin2)
$ cd
$ . pass2   (sets built to use just-built DCC to build DCC again using itself)
$ cd test
$ make clean
$ make      (DCC is now built by DCC in /bin2)
$ make clean
$ make      (DCC is now fully bootstrapped in /bin2, having built itself by itself)
```

Original code located at www.desmet-c.com and [OpenDC](https://github.com/the-grue/OpenDC).
