#
# DeSmet C for ELKS Makefile
#

DIRS = C GEN ASM LIB88 NBIND OTHER

all:
	for DIR in $(DIRS); do $(MAKE) -C $$DIR all || exit 1; done

clean:
	for DIR in $(DIRS); do $(MAKE) -C $$DIR clean || exit 1; done

