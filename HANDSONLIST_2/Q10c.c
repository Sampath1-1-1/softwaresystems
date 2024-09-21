// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 10 of Hands On list 2
// 10. Write a separate program using sigaction system call to catch the following signals.
// a. SIGSEGV
// b. SIGINT
//c. SIGFPE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>  // For pause()

// Signal handler for SIGFPE
void handle_sigfpe(int signal) {
    printf("Caught SIGFPE (Floating-point exception)\n");
    exit(1);  // Exit the program after handling the signal
}

int main() {
    struct sigaction sa;

    // Set up the signal handler
    sa.sa_handler = handle_sigfpe;  // Function to handle the signal
    sa.sa_flags = 0;  // No special flags
    sigemptyset(&sa.sa_mask);  // Initialize the signal mask to empty

    // Set the action for SIGFPE
    if (sigaction(SIGFPE, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Trigger SIGFPE by dividing by zero
   // int x = 1 / 0;  // This will cause a floating-point exception

    // This code will not be reached because of the signal
    raise(SIGFPE);
    printf("This line will not be printed\n");

    return 0;
}