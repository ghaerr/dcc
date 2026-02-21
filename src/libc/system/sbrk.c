#include <unistd.h>

char *sbrk(increment)
int increment;
{
    char *new_brk;

    if (_sbrk (increment, &new_brk)) {
        write(1, "*** _sbrk FAIL\n", 15);
        return (char *) -1;
    }
    return new_brk;
}

