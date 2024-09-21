// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 14 of Hands On list 2
//14. Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>  // Include this header for wait()
#include <string.h>

int main() {
    int pipefd[2];  // Array to hold file descriptors for the pipe
    pid_t pid;
    char buf[50];   // Buffer to hold the data read from the pipe

    // Create the pipe
    if (pipe(pipefd) == -1) {//used to create the pipe
        //pipefd is an array of two integers. pipefd[0] will be used for reading, and pipefd[1] will be used for writing.
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();  // Create a child process

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close unused read end

        // Write to the pipe
        const char *msg = "Hello from child";
        write(pipefd[1], msg, strlen(msg) + 1);//strlen(msg) calculates the length of the string msg, excluding the null terminator ('\0').
        //  Adding 1 to this length includes the null terminator.

        close(pipefd[1]);  // Close the write end
        exit(EXIT_SUCCESS);
    } else {  // Parent process
        close(pipefd[1]);  // Close unused write end

        // Read from the pipe
        read(pipefd[0], buf, sizeof(buf));
        printf("Received message: %s\n", buf);

        close(pipefd[0]);  // Close the read end
        wait(NULL);        // Wait for child process to finish
    }

    return 0;
}
