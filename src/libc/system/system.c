#include <unistd.h>

int wait4(int pid, int *stat_loc, int options, void *usage);

int system(char *command)
{
    int status, pid;

    if (command == 0) return 1;

    if ((pid = fork()) < 0)
        return -1;
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", command, 0);
        _exit(127);
    }

    /* wait for child termination*/
    while (wait4(pid, &status, 0, 0) != pid)
        continue;

    return status;
}
