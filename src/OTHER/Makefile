#
# Host OTHER for ELKS Makefile (dump88, toobj88)
#

############# Standard Section for Host C ##############
CC = ia16-elf-gcc
LD = ia16-elf-gcc

CLBASE =  -melks-libc -mtune=i8086 -Os -mcmodel=small -mno-segment-relocation-stuff
LDBASE =  -melks-libc -mtune=i8086 -Os -mcmodel=small -mno-segment-relocation-stuff

CLBASE += -fpack-struct=1 -fno-inline -fno-builtin
CLBASE += -fno-omit-frame-pointer -fno-optimize-sibling-calls
#CLBASE += -fno-defer-pop

WARNINGS =  -Wno-implicit-function-declaration -Wno-int-conversion
WARNINGS += -Wno-incompatible-pointer-types -Wno-implicit-int

# Optional instrumentation
CLINST = -finstrument-functions-simple
LDINST = -maout-symtab

CFLAGS = $(CLBASE) $(CLINST) $(WARNINGS)
LDFLAGS = $(LDBASE) $(LDINST)

%.o: %.C
	$(CC) -x c -c $(CFLAGS) -o $*.o $<

OBJS = $(SRCS:.C=.o)

############# End of Standard Section ##############

PROGS = dump88 toobj88

.PHONY: $(PROGS)
all: $(PROGS)
	@echo SUCCESS

dump88:
	$(CC) -x c -c $(CFLAGS) DUMPO.C
	$(LD) $(LDFLAGS) -o dump88 DUMPO.o
	@cp dump88 ~greg/net/elks-gh/elkscmd/rootfs_template/root/bin

toobj88:
	$(CC) -x c -c $(CFLAGS) TOOBJ.C
	$(LD) $(LDFLAGS) -maout-heap=32766 -maout-stack=4096 -o toobj88 TOOBJ.o
	@cp toobj88 ~greg/net/elks-gh/elkscmd/rootfs_template/root/bin

clean:
	rm -f $(PROGS) *.o
