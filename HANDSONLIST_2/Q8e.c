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
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

// Signal handler for SIGALRM
void handle_sigalrm(int signal) {
    printf("Caught SIGALRM (Alarm clock using setitimer)\n");
    exit(1);
}

int main() {
    // Set up signal handler
    signal(SIGALRM, handle_sigalrm);

    // Set up SIGALRM using setitimer
    struct itimerval timer;
    timer.it_value.tv_sec = 2;  // Initial value
    timer.it_value.tv_usec = 0;
    // timer.it_interval.tv_sec = 0; // No repeating
    // timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    // Wait for signal
    while (1) {
        pause();  // Pause and wait for the signal
    }

    return 0;
}
