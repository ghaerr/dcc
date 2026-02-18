# C88 misc Makefile
#

INCLUDES=/root/libc/include
#INCLUDES=-I/usr/include -I/usr/include/c86
#DEFINES=
#CPPFLAGS=-0 $(INCLUDES) $(DEFINES)
#CPP=$(BIN)cpp

##### Standardized section of Makefile #####

CC=INCLUDE=$(INCLUDES) $(BIN)c88
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

LIB=misc.a
LIBOBJS=        \
    atol.o      \
    lcmp.o      \
    lcpy.o      \
    lmov.o      \
    lmove.o     \
    ltoa.o      \
    memchr.o    \
    memcmp.o    \
    memcpy.o    \
    memicmp.o   \
    memmove.o   \
    memset.o    \
    peekpoke.o  \
    strcat.o    \
    strchr.o    \
    strcmp.o    \
    strcpy.o    \
    stringi.o   \
    strlen.o    \
    strrchr.o   \
    upper.o     \
    abort.o     \
    bsearch.o   \
    ctype.o     \
    getenv.o    \
    itoa.o      \
    qsort.o     \
    strlwr.o    \
    strrev.o    \
    strset.o    \
    strstr.o    \
    strtok.o    \
    strtol.o    \
    strupr.o    \

all: $(LIB)

$(LIB): $(LIBOBJS)
	$(AR) $^ -O$(LIB)

clean:
	rm -f *.i *.as *.o *.a *.lst
