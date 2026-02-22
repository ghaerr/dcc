# C88 system Makefile
#

#INCLUDES=-I/usr/include -I/usr/include/c86
#DEFINES=
#CPPFLAGS=-0 $(INCLUDES) $(DEFINES)
#CPP=$(BIN)cpp

##### Standardized section of Makefile #####

CC=$(BIN)c88
C2=$(BIN)gen88
AS=$(BIN)asm88
AR=$(BIN)lib88
LD=$(BIN)link88

# Automated rules for C88 toolchain
.c.o:
	#$(CPP) $(CPPFLAGS) $*.c -o $*.i
	$(CC) $*.c PW
	$(C2) $* A
	$(AS) $*.as L

.s.o:
	$(AS) $*.s L

##### End of standardized section #####

LIB=system.a
LIBOBJS=        \
    creat.o     \
    execl.o     \
    execve.o    \
    isatty.o    \
    isetup.o    \
    lseek.o     \
    malloc.o    \
    memory.o    \
    sbrk.o      \
    setjmp.o    \
    stackreg.o  \
    system.o    \

all: $(LIB)

$(LIB): $(LIBOBJS)
	$(AR) $^ -O$(LIB)

clean:
	rm -f *.i *.as *.o *.a *.lst
