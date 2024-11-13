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
#include <unistd.h>
#include <stdlib.h>

// Signal handler for SIGALRM
void handle_sigalrm(int signal) {
    printf("Caught SIGALRM (Alarm clock)\n");
    exit(1);
}

int main() {
    // Set up signal handler
    signal(SIGALRM, handle_sigalrm);

    // Set an alarm to trigger SIGALRM after 5 seconds
    alarm(5);
    //unsigned int alarm(unsigned int seconds);
// After 5 seconds, the operating system will send a SIGALRM signal to the process

    // Wait for signal
    while (1) {
        pause();  // Pause and wait for the signal
        //pause () is used to suspend the execution of the calling process until a signal is delivered
    }

    return 0;
}
