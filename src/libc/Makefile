# C88 libc Makefile
#
MAKE = make
AR = /root/lib88

DIRS = system builtin misc

all:
	for DIR in $(DIRS); do cd $$DIR; $(MAKE) all || exit 1; cd ..; done
	$(AR) */*.a -Ocstdio
	cp cstdio.a /root
	cp cstdio.a /root/test/C

clean:
	for DIR in $(DIRS); do cd $$DIR $(MAKE) clean || exit 1; cd ..; done

