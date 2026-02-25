#include <stdio.h>

unsigned char buferr[BUFSIZ];

FILE  stderr[1] =
{
   {
    buferr,
    buferr,
    buferr,
    buferr,
    buferr + sizeof(buferr),
    2,
    _IONBF | __MODE_WRITE | __MODE_IOTRAN,
    { 0,0,0,0,0,0,0,0 },
    0
   }
};
