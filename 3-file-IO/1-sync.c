//
// Created by root on 1/3/19.
//

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "apue.h"


int main(void)
{
    char greeting[] = "helloworld\n";
    char filename[] = "./sample.txt";
    int fd0, fd1, fd2, ret;
    struct stat status;
    fd0 = open(filename, O_WRONLY | O_CREAT);
    if ((ret = write(fd0, greeting, strlen(greeting))) == -1)
        perror("write error in first try\n");
    else
        printf("write suceeded in first try\n");
    close(fd0);

    stat(filename, &status);
    printf("modification time: %d\n", status.st_mtim);
    fd1 = open(filename, O_WRONLY | O_APPEND | O_SYNC);
    sleep(1);
    if ((ret = write(fd1, greeting, strlen(greeting))) == -1)
        perror("write error in second try\n");
    else
        printf("write suceeded in second try\n");
    stat(filename, &status);
    printf("modification time after write: %d\n", status.st_mtim);
    close(fd1);
    stat(filename, &status);
    printf("modification time after close: %d\n", status.st_mtim);

    fd2 = open(filename, O_WRONLY | O_APPEND | O_DSYNC);
    sleep(1);
    if ((ret = write(fd2, greeting, strlen(greeting))) == -1)
        perror("write error in third try\n");
    else
        printf("write suceeded in third try\n");
    stat(filename, &status);
    printf("modification time after write: %d\n", status.st_mtim);
    close(fd2);
    sleep(1);
    stat(filename, &status);
    printf("modification time after close: %d\n", status.st_mtim);
    printf("%d, %d\n", O_SYNC, O_DSYNC);
    exit(0);
}