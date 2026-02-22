#include <fcntl.h>

int
creat(file, mode)
char *file;
int mode;
{
   return open(file, O_TRUNC|O_CREAT|O_WRONLY, mode);
}
