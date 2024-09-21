// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 15 of Hands On list 2
// 15. Write a simple program to send some data from parent to the child process.
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2]; // pipefd[0] is the read end, pipefd[1] is the write end
    pid_t pid;
    char message[] = "Hii from parent process";
    char buffer[100];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        // Parent process
        close(pipefd[0]); // The parent closes the read end (pipefd[0]) because it only writes to the pipe.
        write(pipefd[1], message, strlen(message) + 1); // Write message to pipe
        close(pipefd[1]); // Close the write end after sending
        printf("Parent: Message sent to child.\n");
    } else {
        // Child process
        close(pipefd[1]); // The child closes the write end (pipefd[1]) because it only reads from the pipe.
        read(pipefd[0], buffer, sizeof(buffer)); // Read message from pipe
        close(pipefd[0]); // Close the read end after reading
        printf("Child: Message is received - %s\n", buffer);
    }

    return 0;
}

