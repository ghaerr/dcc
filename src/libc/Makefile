# C88 libc Makefile
#
MAKE = make
AR = $(BIN)lib88

DIRS = stdio system builtin misc

all:
	for DIR in $(DIRS); do cd $$DIR; $(MAKE) all || exit 1; cd ..; done
	$(AR) */*.o -Ocstdio

clean:
	for DIR in $(DIRS); do cd $$DIR; $(MAKE) clean || exit 1; cd ..; done

