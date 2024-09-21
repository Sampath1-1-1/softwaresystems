// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 5 of Hands On list 2
// 5. Write a program to print the system limitation of
// a. maximum length of the arguments to the exec family of functions.
// b. maximum number of simultaneous process per user id.
// c. number of clock ticks (jiffy) per second.
// d. maximum number of open files
// e. size of a page
// f. total number of pages in the physical memory
// g. number of currently available pages in the physical memory.
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    // a. Maximum length of arguments to exec family
    long arg_max = sysconf(_SC_ARG_MAX);
    printf("Maximum length of arguments to exec family: %ld bytes\n", arg_max);

    // b. Maximum number of simultaneous processes per user
    long max_processes = sysconf(_SC_CHILD_MAX);
    printf("Maximum number of simultaneous processes per user: %ld\n", max_processes);

    // c. Number of clock ticks (jiffy) per second
    long clock_ticks = sysconf(_SC_CLK_TCK);
    printf("Number of clock ticks per second: %ld\n", clock_ticks);

    // d. Maximum number of open files
    struct rlimit rl;
    getrlimit(RLIMIT_NOFILE, &rl);
    printf("Maximum number of open files: %ld\n", rl.rlim_cur);

    // e. Size of a page
    long page_size = sysconf(_SC_PAGESIZE);
    printf("Size of a page: %ld bytes\n", page_size);

    // f. Total number of pages in physical memory
    long total_pages = sysconf(_SC_PHYS_PAGES);
    printf("Total number of pages in physical memory: %ld\n", total_pages);

    // g. Number of currently available pages in physical memory
    long available_pages = sysconf(_SC_AVPHYS_PAGES);
    printf("Currently available pages in physical memory: %ld\n", available_pages);

    return 0;
}


