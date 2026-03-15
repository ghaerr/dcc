#
# Host 'make' make for ELKS Makefile
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
#CLINST = -finstrument-functions-simple
#LDINST = -maout-symtab

CFLAGS = $(CLBASE) $(CLINST) $(WARNINGS) $(LOCALFLAGS)
LDFLAGS = $(LDBASE) $(LDINST)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $*.o $<

OBJS = $(SRCS:.c=.o)

############# End of Standard Section ##############

SRCS = check.c input.c macro.c main.c make.c reader.c rules.c
PROG = make

LOCALFLAGS = -Dunix
LDFLAGS += -maout-heap=16384 -maout-stack=4096

all: $(PROG)

.PHONY: $(PROG)
$(PROG):
	$(CC) -c $(CFLAGS) $(SRCS)
	$(LD) $(LDFLAGS) -o $(PROG) $(OBJS)
	#cp $(PROG) ~greg/net/elks-gh/elkscmd/rootfs_template/root/bin
	@echo SUCCESS

clean:
	rm -f $(PROG) $(OBJS)
