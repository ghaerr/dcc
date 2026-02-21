#include <time.h>

/* ELKS stat struct */
struct stat {
    unsigned        st_dev;
    unsigned long   st_ino;
    unsigned        st_mode;
    unsigned        st_nlink;
    unsigned        st_uid;
    unsigned        st_gid;
    unsigned        st_rdev;
    long            st_size;
    time_t          st_atime;
    time_t          st_mtime;
    time_t          st_ctime;
};

int stat(char *path, struct stat *buf);
