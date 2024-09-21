// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 17 of Hands On list 2
// 17. Write a program to execute ls -l | wc.
// a. use dup
// b. use dup2
// c. use fcntl
#include <stdio.h>
#include <unistd.h>

int main() {
    int pipefd[2];
    pid_t pid;

    // Create a pipe
    pipe(pipefd);

    // Fork a new process
    pid = fork();

    if (pid == 0) {
        // Child process - executes `ls -l`
        close(pipefd[0]);                // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to the write end of the pipe
        close(pipefd[1]);                // Close the original write end
        execlp("ls", "ls", "-l", NULL);   // Execute `ls -l`
    } else {
        // Parent process - executes `wc`
        close(pipefd[1]);                // Close the write end of the pipe
        dup2(pipefd[0], STDIN_FILENO);   // Redirect stdin to the read end of the pipe
        close(pipefd[0]);                // Close the original read end
        execlp("wc", "wc", NULL);         // Execute `wc`
    }

    return 0;
}
