# C88 libc Makefile
#
MAKE = make
AR = $(BIN)lib88

DIRS = system builtin misc

all:
	for DIR in $(DIRS); do cd $$DIR; $(MAKE) all || exit 1; cd ..; done
	$(AR) */*.a -Ocstdio
	cp cstdio.a /root
	cp cstdio.a /root/test/C
	cp cstdio.a /root/test/GEN
	cp cstdio.a /root/test/ASM
	cp cstdio.a /root/test/NBIND
	cp cstdio.a /root/test/LIB88
	cp cstdio.a /root/test/OTHER

clean:
	for DIR in $(DIRS); do cd $$DIR; $(MAKE) clean || exit 1; cd ..; done

