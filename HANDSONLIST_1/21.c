// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 21 of Hands On list1
//21.Write a program, call fork and print the parent and child process id.
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>  // For fork() and getpid()

int main() {
    pid_t pid;  // Variable to store the process ID

    // Create a new process
    pid = fork();
    //    fork() creates a new process. The new process (child) is a copy of the calling process (parent).
    // It returns:
    //     0 to the child process.
    //     The child's PID to the parent process.
    //     -1 if the fork failed.

 // Check if fork() was successful
    if (pid < 0) {
        // fork() failed
        perror("fork failed");
        return 1;  // Exit the program with an error status
    }

    // Check if we are in the parent or the child process
    if (pid == 0) {
        // This block is executed by the child process
        printf("Child Process:\n");
        printf("Child Process ID (PID): %d\n", getpid());
        printf("Parent Process ID (PID): %d\n", getppid());
//         Prints its own PID using getpid().
// Prints the PID of its parent using getppid().
// To the Parent Process: fork() returns the PID of the newly created child process. This allows the parent to keep track of the child and manage it if needed.
// To the Child Process: fork() returns 0. This signals to the child process that it is indeed the child process created by the fork() call.
// Positive Integer: If fork() is successful and the process is the parent, pid holds the PID of the child process.
// Zero (0): If fork() is successful and the process is the child, pid is 0.
// Negative Integer (-1): If fork() fails, pid is -1, and you should handle this as an error.

    } else {
        // This block is executed by the parent process
        printf("Parent Process:\n");
        printf("Parent Process ID (PID): %d\n", getpid());
        printf("Child Process ID (PID): %d\n", pid);//it s parent process so it holds process id of child process
        //parent process will run first it will dead .then the child process starts running 
        //sometimes parent pid of child process  is process id of systemd which is init process.when child process executes getppid() which gives the pid of init process because parent process is dead.
    }
//init is the traditional first process started by the Linux kernel when the system boots. 
// Its process ID (PID) is 1, and it is the ancestor of all other processes.
// The primary role of init is to initialize the system, manage system services, and handle process management.
//systemd is a modern initialization system and service manager designed to replace the older init systems (like SysVinit and Upstart).
    return 0;  // Exit the program with a success status
}
// ps -p pid gives information about this process
// If a parent process ends and the child process continues running, the child process will be re-parented to init or another process. 
// This ensures that the child process has a valid parent for proper process management.
// // if a child process executes before its parent process terminates, 
// the parent PID (PPID) of that child process does not change to the systemd PID or any other process PID.