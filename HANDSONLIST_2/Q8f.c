// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 8 of Hands On list 2
// 8. Write a separate program using signal system call to catch the following signals.
// a. SIGSEGV
// b. SIGINT
// c. SIGFPE
// d. SIGALRM (use alarm system call)
// e. SIGALRM (use setitimer system call)
// f. SIGVTALRM (use setitimer system call)
// g. SIGPROF (use setitimer system call)
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handler_sigvtalrm(int signum) {
    printf("Caught SIGVTALRM: Virtual timer alarm  is triggered!\n");
    exit(0);
}

int main() {
    struct itimerval timer;
    signal(SIGVTALRM,handler_sigvtalrm);

    timer.it_value.tv_sec = 2;  // Initial delay of 2 seconds
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0; // No repeat
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    printf("Waiting for virtual alarm...\n");
    while (1); // Wait indefinitely
    return 0;
}
