#include <time.h"

struct utimbuf {
    unsigned long  actime;
    unsigned long  modtime;
};

int utime(char *filename, struct utimbuf *utimebuf);
