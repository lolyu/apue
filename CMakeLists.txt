cmake_minimum_required(VERSION 3.12)
project(apue C)

set(CMAKE_C_STANDARD 11)

add_executable(apue
        1-unix-system-overview/01_ls.c
        1-unix-system-overview/02_redirect.c
        1-unix-system-overview/03_redirect.c
        1-unix-system-overview/04_error.c)
