# C88 stdio Makefile
#

#INCLUDES=/root/libc/include
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
	$(C2) $* Z
	#$(AS) $*.as L

.s.o:
	$(AS) $*.s L

##### End of standardized section #####

LIB=stdio.a
LIBOBJS=        \
    __fopen.o   \
    fclose.o    \
    fflush.o    \
    fgetc.o     \
    fgets.o     \
    fopen.o     \
    fprintf.o   \
    fputc.o     \
    fputs.o     \
    fread.o     \
    fwrite.o    \
    init.o      \
    printf.o    \
    stdin.o     \
    stdout.o    \
    stderr.o    \
    vfprintf.o  \

all: $(LIB)

$(LIB): $(LIBOBJS)
	$(AR) $^ -O$(LIB)

clean:
	rm -f *.i *.as *.o *.a *.lst
