//
// Created by root on 1/18/19.
//

//
// Created by root on 1/17/19.
//

#include "apue.h"
#include <dirent.h>
#include <limits.h>

// #define NAME_MAX         255	/* # chars in a file name */
// #define PATH_MAX        4096	/* # chars in a path name including nul */

typedef int Myfunc(const char *, const struct stat *, int);

// static function variable keeps value between invocations
// static global value is seen only in the file it's declared in
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *, const char *);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;


int main(int argc, char *argv[])
{
    int ret;
    if (argc != 2)
        err_quit("usage: ftw <starting-pathname>");
    char *current = malloc(sizeof(char) * PATH_MAX);
    if (getcwd(current, PATH_MAX) == NULL)
        err_sys("failed to get current working directory");
    ret = myftw(argv[1], myfunc);
    chdir(current);
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1;
    printf("regular files = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
    exit(ret);
}

#define FTW_F   1   // file other than dir
#define FTW_D   2   // dir
#define FTW_DNR 3   // directory can't read
#define FTW_NS  4   // file can't stat


static int myftw(char *pathname, Myfunc *func)
{
    return(dopath(func, pathname));
}

static int dopath(Myfunc *func, const char *path)
{
    struct stat     statbuf;
    struct dirent   *dirp;
    DIR             *dp;
    int             ret, n;
    if (lstat(path, &statbuf) < 0)
        return(func(path, &statbuf, FTW_NS));
    if (S_ISDIR(statbuf.st_mode) == 0)
        return(func(path, &statbuf, FTW_F));
    if ((ret = func(path, &statbuf, FTW_D)) != 0)
        return(ret);
    if ((dp = opendir(path)) == NULL)
        return(func(path, &statbuf, FTW_DNR));
    chdir(path);
    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;
        if ((ret = dopath(func, dirp->d_name)) != 0)
            break;
    }
    chdir("..");
    if (closedir(dp) < 0)
        err_ret("can't close directory %s", path);
    return(ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch (type)
    {
        case FTW_F:
            // S_IFMT is a bit mask for file type
            // stat.st_mode & S_IFMT to determine file type
            // stat.st_mode & ~S_IFMT to determine file permissions(other than file type)
            switch (statptr->st_mode & S_IFMT)
            {
                case S_IFREG: ++nreg;       break;
                case S_IFBLK: ++nblk;       break;
                case S_IFCHR: ++nchr;       break;
                case S_IFIFO: ++nfifo;      break;
                case S_IFLNK: ++nslink;     break;
                case S_IFSOCK: ++nsock;     break;
                case S_IFDIR:
                    err_dump("for S_IFDIR for %s", pathname);
            }
            break;
        case FTW_D:
            ++ndir;
            break;
        case FTW_DNR:
            err_ret("can't read directory %s", pathname);
            break;
        case FTW_NS:
            err_ret("stat error for %s", pathname);
            break;
        default:
            err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return(0);
}