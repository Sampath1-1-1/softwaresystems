// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 11 of Hands On list 2
//11. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - use sigaction system call.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>  // For pause()

// Signal handler for SIGINT (initially set to ignore)
void handle_sigint(int signal) {
    printf("SIGINT received but will be ignored.\n");
}

// Function to reset SIGINT to default action
void reset_sigint() {
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;  // Reset to default action
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

int main() {
    struct sigaction sa;

    // Set up the signal handler to ignore SIGINT
    sa.sa_handler = SIG_IGN;  // Ignore the signal
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("SIGINT signal is now ignored. Try pressing Ctrl+C.\n");

    // Wait for 5 seconds
    sleep(5);

    // Reset SIGINT to default action
    reset_sigint();
    printf("SIGINT signal action reset to default.\n");

    // Wait indefinitely to see the effect of the default action
    while (1) {
        pause();  // Wait for signals
    }

    return 0;
}

