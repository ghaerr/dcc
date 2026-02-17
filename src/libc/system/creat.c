#define O_WRONLY        01
#define O_CREAT       0100
#define O_TRUNC      01000

int
creat(file, mode)
char *file;
int mode;
{
   return open(file, O_TRUNC|O_CREAT|O_WRONLY, mode);
}
