//
// Created by root on 1/17/19.
//
#include "apue.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>


/*
# truncate --size 1G bigfile
# ./a.out bigfile bigfile1
# ls -la bigfile*
-rw-r--r-- 1 root root 1073741824 Jan 17 18:43 bigfile
-rw-r--r-- 1 root root          0 Jan 17 18:48 bigfile1
# du -h bigfile*
0	bigfile
0	bigfile1
 */


bool is_all_zeros(char *buf, size_t len);


int main(int argc, char *argv[])
{
    if (argc != 3)
        err_quit("usage: mycp <source> <dest>");

    int src, dest;
    int buffer_size;
    char *buf;
    struct stat statbuf0, statbuf1;
    if ((src = open(argv[1], O_RDONLY)) < 0)
        err_sys("failed to open source %s", argv[1]);
    if (stat(argv[1], &statbuf0) < 0)
        err_sys("failed to stat source %s", argv[1]);
    if ((dest = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, statbuf0.st_mode)) < 0)
        err_sys("failed to open dest %s", argv[2]);
    if (stat(argv[2], &statbuf1) < 0)
        err_sys("failed to stat dest %s", argv[2]);
    buffer_size = statbuf1.st_blksize;
    if ((buf = malloc(sizeof(char) * buffer_size)) == NULL)
        err_sys("failed to allocate buffer");


    size_t nread;
    do {
        ssize_t m = 0;
        nread = 0;
        do {
            // read return -1 for error, 0 for EOF, positive for number of bytes read
            if ((m = read(src, buf + nread, buffer_size - nread)) < 0)
                err_sys("failed to read from source %s", argv[1]);
            nread += m;
        } while (nread < buffer_size && m != 0);
        if (nread == buffer_size && is_all_zeros(buf, buffer_size)) {
            if (lseek(dest, buffer_size, SEEK_CUR) < 0) {
                err_sys("failed to lseek in dest %s", argv[2]);
            }
        } else {
            ssize_t nwritten = 0;
            while (nwritten < nread) {
                if ((m = write(dest, buf + nwritten, nread - nwritten)) < 0)
                    err_sys("failed to write to dest %s", argv[2]);
                nwritten += m;
            }
        }
    } while (nread > 0);
    if (stat(argv[2], &statbuf1) < 0)
        err_sys("failed to stat dest %s", argv[2]);
    // if source is an empty file, truncate dest directly to src size
    if (statbuf1.st_size == 0)
        if (truncate(argv[2], statbuf0.st_size) < 0)
            err_sys("failed to truncate %s", argv[2]);
    if (close(src) < 0)
        err_sys("failed to close source %s", argv[1]);
    if (close(dest) < 0)
        err_sys("failed to close dest %s", argv[2]);
    free(buf);
    exit(0);
}


bool is_all_zeros(char *buf, size_t len)
{
    int i;
    for (i = 0; i < len; ++i)
        if (buf[i] != 0)
            return false;
    return true;
}