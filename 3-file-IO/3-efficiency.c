#include "apue.h"
#include <fcntl.h>

#define BUFSIZE 4096

void set_fl(int fd, int flags)
{
    int val = 0;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");
    val |= flags;
    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}


int main(void)
{
    int n;
    char buf[BUFSIZE];
    set_fl(STDOUT_FILENO, O_SYNC);
    while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
        // fsync(STDOUT_FILENO);
    if (n < 0)
        err_sys("read error");
    exit(0);
}