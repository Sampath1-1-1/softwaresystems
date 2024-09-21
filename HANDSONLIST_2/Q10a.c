// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 10 of Hands On list 2
// 10. Write a separate program using sigaction system call to catch the following signals.
// a. SIGSEGV
// b. SIGINT
////c. SIGFPE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>  // For sleep()

// Signal handler for SIGSEGV
void handle_sigsegv(int signal) {
    printf("Caught SIGSEGV (Segmentation fault)\n");
    exit(1);  // Exit the program with a failure status
}

int main() {
    struct sigaction sa;

    // Set up the signal handler
    sa.sa_handler = handle_sigsegv;  // Function to handle the signal
    //This member of struct sigaction is a pointer to a signal handling function. It specifies what 
    // function should be executed when the signal occurs.
    sa.sa_flags = 0;  // No special flags
    sigemptyset(&sa.sa_mask);  // Initialize the signal set to empty

    // Set the action for SIGSEGV
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Trigger SIGSEGV by dereferencing a NULL pointer
    int *ptr = NULL;
  *ptr = 42;  // This will cause a segmentation fault

    return 0;

}
