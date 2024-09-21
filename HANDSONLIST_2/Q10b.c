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

// Signal handler for SIGINT
void handle_sigint(int signal) {
    printf("Caught SIGINT (Interrupt signal)\n");
    exit(0);  // Exit gracefully after handling the signal
}

int main() {
    struct sigaction sa;

    // Set up the signal handler
    sa.sa_handler = handle_sigint;  // Function to handle the signal
    sa.sa_flags = 0;  // No special flags
    sigemptyset(&sa.sa_mask);  // Initialize the signal set to empty

    // Set the action for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Wait for signal
    printf("Waiting for SIGINT. Press Ctrl+C to send SIGINT.\n");
    while (1) {
        pause();  // Pause and wait for signals
    }

    return 0;
}
