# DeSmet C for ELKS
The 1989 DOS DeSmet C toolchain v3.10h (C88, GEN88, ASM88, LIB88, NBIND, DUMPO and TOOBJ)
ported to [ELKS](https://github.com/ghaerr/elks).

A work in progress. The goal is to produce an ANSI C capable compiler toolchain that can compile itself
on 8086 systems.
This could then be used for creating self-reproducing systems based on the 8086, including
ELKS or
[UnixV6-8086](https://github.com/ghaerr/UnixV6-8086).

DCC is now able to compile itself and run under ELKS.
The next steps will bootstrap the DCC toolchain by compiling on the host system,
after which the DCC source will be copied to the ELKS /usr/dcc directory:
```
$ cd /path/to/ELKS
$ . env.sh  (sets TOPDIR= and PATH= to ia16-elf-gcc toolchain)
$ cd /path/to/DCC
$ make
$ ./copyc88.sh
$ cd /path/to/ELKS
$ cd image
$ make hd32-minix
```
The DCC source should now be in the /usr/dcc directory for the 32MB HDD image
just built on ELKS.

Now we're ready to actually build DCC on ELKS. Boot the hd32-minix.img in the
last step above, and login as root. Then execute the following to fully bootstrap the
DCC compiler:
```
# cd /usr/dcc
# . pass1       (sets build to use /usr/dcc/bin directory for DCC compiled by ia16-elf-gcc)
# make          (builds DCC using gcc-built DCC into /usr/dcc/bin2)
#               (DCC should now be built in /usr/dcc/bin2)
# . pass2       (sets build to use just-built DCC to build DCC again using itself)
# bin/make clean
# bin/make      (DCC is now built by DCC in /usr/dcc/bin2)
# bin/make clean
# bin/make      (DCC is now fully bootstrapped in usr/dcc/bin2, having built itself by itself)
```

Original 1989 source code located at www.desmet-c.com and [OpenDC](https://github.com/the-grue/OpenDC).
