#define TCGETS          (('T'<<8)+0x01)

typedef long            tcflag_t;
typedef unsigned char   cc_t;

#define NCCS 17
struct termios {
    tcflag_t c_iflag;       /* input mode flags */
    tcflag_t c_oflag;       /* output mode flags */
    tcflag_t c_cflag;       /* control mode flags */
    tcflag_t c_lflag;       /* local mode flags */
    cc_t c_line;            /* line discipline */
    cc_t c_cc[NCCS];        /* control characters */
};

#define ENOSYS          38      /* Function not implemented */
extern int errno;

int
isatty(int fd)
{
    int rv, err = errno;
    struct termios term;

    rv = (ioctl(fd, TCGETS, &term) == 0);

    if(rv == 0 && errno == ENOSYS)
        rv = (fd < 3);

    errno = err;
    return rv;
}
