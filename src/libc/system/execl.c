#include <unistd.h>

int
execl(fname, arg0)
char *fname, *arg0;
{
    return execve(fname, &arg0, environ);
}
