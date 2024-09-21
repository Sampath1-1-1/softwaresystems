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

// Signal handler for SIGSEGV
void handle_sigsegv(int signal) {
    printf("Caught SIGSEGV (Segmentation fault)\n");
    exit(1);
}

int main() {
    // Set up signal handler
    signal(SIGSEGV, handle_sigsegv);//SIGSEGV -Segmentation Violation" signal.
//     void (*signal(int sig, void (*handler)(int)))(int);
// sig: The signal number (e.g., SIGSEGV).
// handler: A pointer to the function that will handle the signal.
    // Trigger SIGSEGV by dereferencing a NULL pointer
    int *ptr = NULL;
    *ptr = 42;  // This will cause a segmentation fault
//This signal is sent to a process when it makes an invalid memory reference,
// such as dereferencing a null or uninitialized pointer.
    return 0;
}
//NULL is a special constant that represents a null pointer, meaning the pointer does not point to any valid memory address.