// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 23 of Hands On list1
//23.Write a program to create a Zombie state of the running program.
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
// when parent process is busy somewhere else not able to get the query of the child process then child process is become zombie
// In the case of a terminated child, performing a wait allows
//        the system to release the resources associated with  the  child;  if  a
//        wait  is not performed, then the terminated child remains in a "zombie"
    //    state
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
// it's important to ensure that parent processes wait for their children to terminate using functions like 
// wait or waitpid.