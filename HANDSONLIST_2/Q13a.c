// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 13 of Hands On list 2
//13. Write two programs: first program is waiting to catch SIGSTOP signal, the second program
// will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGSTOP (won't be called)
void handle_sigstop(int signal) {
    printf("Caught SIGSTOP (this should not happen)\n");
}

int main() {
    // Set up signal handler (for demonstration, but it won't be effective)
    signal(SIGSTOP, handle_sigstop);
    
    printf("Waiting for SIGSTOP signal. My PID is %d.\n", getpid());
    
    // Infinite loop to keep the program running
    while (1) {
        pause();  // Wait for a signal
    }
    
    return 0;
}
