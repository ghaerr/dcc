#include <stdio.h>

unsigned char bufout[BUFSIZ];

FILE  stdout[1] =
{
   {
    bufout,
    bufout,
    bufout,
    bufout,
    bufout + sizeof(bufout),
    1,
    _IOLBF | __MODE_WRITE,      /* FIXME flush on exit to fix */
    { 0,0,0,0,0,0,0,0 },
    0
   }
};
