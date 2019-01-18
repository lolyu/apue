//
// Created by root on 1/4/19.
//

#include "apue.h"
#include <fcntl.h>

char buf0[] = "abcdefghij";
char buf1[] = "ABCDEFGHIJ";


int main(void)
{
    int fd;

    if ((fd = creat("filehole", FILE_MODE)) < 0)
        err_sys("create error");
    if (write(fd, buf0, 10) != 10)
        err_sys("buf0 write error");
    if (lseek(fd, 16384, SEEK_SET) == -1)
        err_sys("lseek error");
    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error");
    exit(0);
}

