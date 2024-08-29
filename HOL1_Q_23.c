// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 23 of Hands On list1
//23.Write a program to create a Zombie state of the running program.
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

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

        // The child process exits immediately
        exit(0);  // This makes the child process exit and become a zombie
        //The child process is called a zombie because it has completed execution, but its entry in 
        //the process table remains until the parent process retrieves the exit status using a system call like wait().
    } else {
        // This block is executed by the parent process
        printf("Parent Process:\n");
        printf("Parent PID: %d\n", getpid());
        printf("Waiting for 10 seconds to create a zombie process...\n");

        // Sleep for 10 seconds to allow the child process to exit and become a zombie
        sleep(10);
        //If the parent process were to immediately call wait(), 
        //it would collect the exit status of the child immediately, and the child would not have the opportunity to become a zombie.

        // The parent process can now collect the exit status of the child
        printf("Parent Process: Reaping the zombie child process...\n");
        wait(NULL);  // Wait for the child process to terminate and clean up the zombie

        printf("Parent Process: Zombie process cleaned up.\n");
    }

    return 0;
}
