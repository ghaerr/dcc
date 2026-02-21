#include <unistd.h>

char *sbrk(increment)
int increment;
{
    char *new_brk;

    if (_sbrk (increment, &new_brk))
        return (char *) -1;
    return new_brk;
}

