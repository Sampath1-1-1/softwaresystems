// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 13 of Hands On list 2
//13. Write two programs: first program is waiting to catch SIGSTOP signal, the second program
// will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    // Replace with the actual PID of the first program
    pid_t pid;
    printf("Enter the PID of the process to stop: ");
    scanf("%d", &pid);

    // Send SIGSTOP to the specified process
    if (kill(pid, SIGSTOP) == -1) {
        perror("Failed to send SIGSTOP");
        exit(1);
    }

    printf("Sent SIGSTOP to process %d.\n", pid);
    
    return 0;
}
