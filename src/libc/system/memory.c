#include <unistd.h>

static unsigned heapsize;

/* allocate max memory from heap */
_memory() {
    unsigned curbrk, newbrk;

    curbrk = sbrk(0);
    newbrk = __stacklow - 512;
    heapsize = newbrk - curbrk;
    if (_brk(newbrk)) {
        write(1, "No heap mem\n", 12);
        _exit(1);
    }
    return curbrk;
}

_memsize() {
    return heapsize;
}
