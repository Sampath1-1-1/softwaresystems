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
#include <stdlib.h>
#include <unistd.h>
// Signal handler for SIGINT
void handle_sigint(int signal) {
    printf("Caught SIGINT (Interrupt from keyboard)\n");
    exit(1);
}

int main() {
    // Set up signal handler
    signal(SIGINT, handle_sigint);//SIGINT stands for signal interrupt
// It is used to request the termination of a process or program. 
// The signal is sent by the terminal when the user interrupts the process.
    // Wait for signal
    while (1) {
        printf("Running... Press Ctrl+C to interrupt.\n");
        sleep(1);
    }

    return 0;
}
//When you run a program in a terminal and you press Ctrl+C, the terminal sends a SIGINT signal to the currently running process. 
// This is a way for users to stop a running program that might be stuck or taking too long to complete.
