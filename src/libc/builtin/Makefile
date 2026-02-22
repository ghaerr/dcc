# C88 builtin Makefile
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

LIB=builtin.a
LIBOBJS=        \
    cmp4.o      \
    cmp4u.o     \
    sar4.o      \
    shl4.o      \
    shr4.o      \
    switch.o    \
    muldiv.o    \
    umuldiv.o   \

all: $(LIB)

$(LIB): $(LIBOBJS)
	$(AR) $^ -O$(LIB)

clean:
	rm -f *.i *.as *.o *.a *.lst
