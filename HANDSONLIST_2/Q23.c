// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 23 of Hands On list 2
// 23. Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    // Part 1: Maximum number of files a process can open
    struct rlimit rl;// used with system calls to get or set resource limits for processes
    
    if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
        printf("Maximum number of open files for this process: %ld\n", rl.rlim_cur);
    } else {
        perror("getrlimit failed");
        return 1;
    }

    // Part 2: Size of the pipe (circular buffer)
    int pipe_fds[2];
    
    // Create a pipe
    if (pipe(pipe_fds) == -1) {
        perror("pipe failed");
        return 1;
    }

    // Use fcntl to get the pipe buffer size
    int pipe_size = fcntl(pipe_fds[0], F_GETPIPE_SZ);// to get the size of the pipe's buffer in bytes.
    if (pipe_size == -1) {
        perror("fcntl failed");
        return 1;
    }

    printf("Size of the pipe (circular buffer): %d bytes\n", pipe_size);

    // Close the pipe file descriptors
    close(pipe_fds[0]);
    close(pipe_fds[1]);

    return 0;
}

// The buffer operates in a circular manner. When the end of the buffer is reached, it wraps around to the beginning. 
// This is managed using two pointers or indices: one for writing (producing data) and one for reading (consuming data).