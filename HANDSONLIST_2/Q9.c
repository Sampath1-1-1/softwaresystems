// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 9 of Hands On list 2
//9. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep() and pause()

// Signal handler for SIGINT
void handle_sigint(int signal) {
    printf("Caught SIGINT signal. This should not be printed initially.\n");
}

// Function to ignore SIGINT
void ignore_sigint() {
    signal(SIGINT, SIG_IGN);
    //SIG_IGN: This is a constant that tells the system to ignore the specified signal. 
    printf("SIGINT signal is now ignored. Try pressing Ctrl+C.\n");
}

// Function to reset SIGINT to default
void reset_sigint() {
    signal(SIGINT, SIG_DFL);
    //SIG_DFL stands for "Signal Default". It is used to indicate that the default action for a signal should be used
    printf("SIGINT signal handling reset to default. Press Ctrl+C to exit.\n");
}

int main() {
    // Initially ignore SIGINT
    ignore_sigint();

    // Sleep for 10 seconds to allow time for user to test ignoring SIGINT
    sleep(10);

    // Reset SIGINT to default action
    reset_sigint();

    // Wait for the signal to observe the default behavior
    while (1) {
        pause();  // Pause and wait for signals
    }

    return 0;
}

