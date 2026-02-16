# libc Makefile
#

MAKE = /root/make
AR = /root/lib88
DIRS = builtin misc

all:
	for DIR in $(DIRS); do cd $$DIR; $(MAKE) all || exit 1; cd ..; done
	$(AR) */*.a -Ocstdio
	cp cstdio.a /root

clean:
	for DIR in $(DIRS); do cd $$DIR $(MAKE) clean || exit 1; cd ..; done

