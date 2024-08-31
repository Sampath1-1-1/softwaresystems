// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 24 of Hands On list1
//24.Write a program to create an orphan process.
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
//Creating an orphan process involves making a child process that outlives its parent process. 
// An orphan process is one that continues to run after its parent process has terminated. 
// The init process (or systemd in modern systems) will adopt such orphan processes and manage them

int main() {
    pid_t pid;

    // Create a new process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // This block is executed by the child process
        printf("Child Process:\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());

        // Sleep for 10 seconds to give time for the parent process to terminate
        printf("Child Process: Sleeping for 10 seconds...\n");
        sleep(10);

        // After sleeping, print the parent PID again to show it has changed
        printf("Child Process: Parent PID after sleep: %d\n", getppid());
    } else {
        // This block is executed by the parent process
        printf("Parent Process:\n");
        printf("Parent PID: %d\n", getpid());

        // The parent process exits immediately
        printf("Parent Process: Exiting...\n");
        exit(0);
    }

    return 0;
}
