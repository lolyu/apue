#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "apue.h"


int mydup2(int oldfd, int newfd)
{
    struct stat s;

    if (fstat(oldfd, &s) < 0)
    {
        errno = EBADF;
        return -1;
    }
    int openmax = sysconf(_SC_OPEN_MAX);
    if (newfd < 0 || newfd > openmax)
    {
        errno = EBADF;
        return -1;
    }
    if (oldfd == newfd)
        return newfd;
    if (fstat(newfd, &s) == 0)
        close(newfd);

    // loop to get duplicated descriptor same
    // to newfd
    int index = 0;
    int *allocated = malloc(sizeof(int) * newfd);
    int current;
    int i;
    while (1)
    {
        if ((current = dup(oldfd)) < 0)
            err_sys("dup failed");
        if (current == newfd)
        {
            for (i = 0; i < index; ++i)
                close(allocated[i]);
            free(allocated);
            return newfd;
        }
        else
            allocated[index++] = current;
    }
}


int main(int argc, char *argv[]) {
    int fd0, fd1;
    if (argc != 2)
        err_quit("Usage: %s file", argv[0]);
    if ((fd0 = open(argv[1], O_RDONLY)) < 0)
        err_sys("can't open file %s", argv[1]);
    fd1 = mydup2(fd0, 100);
    printf("%d\n", fd1);
}