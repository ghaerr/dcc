#ifndef __TIME_H
#define __TIME_H

typedef unsigned long    time_t;

struct timeval {
    long tv_sec;
    long tv_usec;
};

time_t time(time_t *tloc);

int gettimeofday(struct timeval *tp, void *tzp);    /* syscall */

#endif
