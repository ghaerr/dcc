/* unistd.h */

extern char **environ;
extern unsigned __stacklow;

int read(int fd, void *buf, unsigned nbytes);
int write(int fd, void *buf, unsigned n);
int close(int fildes);

int isatty(int fd);

void *sbrk(int increment);
int _sbrk(int increment, void **pnewbrk);       /* syscall */
int _brk (unsigned int newbrk);                 /* syscall */

int fork(void);

int execl(char *fname, char *arg0, ...);
int execve(char *fname, char **argv, char **envp);
int _execve(char *fname, char *stk_ptr, int stack_bytes);   /* syscall */

long lseek(int fildes, long offset, int whence);
int _lseek(int fd, long *posn, int where);      /* syscall */
