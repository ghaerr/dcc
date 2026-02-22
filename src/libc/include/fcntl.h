/* open/fcntl */

#define O_ACCMODE     0003
#define O_RDONLY        00
#define O_WRONLY        01
#define O_RDWR          02
#define O_CREAT       0100  /* not fcntl */
#define O_EXCL        0200  /* not fcntl */
#define O_NOCTTY      0400  /* not fcntl */
#define O_TRUNC      01000  /* not fcntl */
#define O_APPEND     02000
#define O_NONBLOCK   04000
#define O_NDELAY    O_NONBLOCK

#define F_DUPFD     0   /* dup */
#define F_GETFD     1   /* get cloexec flag */
#define F_SETFD     2   /* set cloexec flag */
#define F_GETFL     3   /* get f_flags */
#define F_SETFL     4   /* set f_flags (O_APPEND, O_NONBLOCK only) */

#define FD_CLOEXEC  1   /* for F_GETFD, F_SETFD */

int creat(const char *filename, unsigned mode);
int fcntl(int fildes,int cmd, ...);
int open(const char *filename, int flags, ...);    /* syscall */
