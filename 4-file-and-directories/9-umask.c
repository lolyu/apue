//
// Created by root on 1/18/19.
//

#include "apue.h"
#include <fcntl.h>


int main(void)
{
    char filename0[] = "creat.file";
    char filename1[] = "open.file";
    int fd0 = creat(filename0, 0777);
    int fd1 = open(filename1, O_CREAT | O_WRONLY, 0777);
    exit(0);
}