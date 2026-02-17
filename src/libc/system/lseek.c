int _lseek(int fd, long *posn, int where);

long
lseek(fd, posn, where)
int fd;
long posn;
int where;
{
    if (_lseek (fd, &posn, where) < 0)
        return -1;
    return posn;
}

