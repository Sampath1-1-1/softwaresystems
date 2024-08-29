// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 25 of Hands On list1
//25.Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call).
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid1, pid2, pid3;

    // Create the first child process
    if ((pid1 = fork()) == 0) {
        // First child process
        printf("Child 1 (PID: %d) is running.\n", getpid());
        sleep(1);
        printf("Child 1 (PID: %d) is done.\n", getpid());
        exit(EXIT_SUCCESS);
    } else if (pid1 < 0) {
        perror("Failed to fork Child 1");
        exit(EXIT_FAILURE);
    }

    // Create the second child process
    if ((pid2 = fork()) == 0) {
        // Second child process
        printf("Child 2 (PID: %d) is running.\n", getpid());
        sleep(2);
        printf("Child 2 (PID: %d) is done.\n", getpid());
        exit(EXIT_SUCCESS);
    } else if (pid2 < 0) {
        perror("Failed to fork Child 2");
        exit(EXIT_FAILURE);
    }

    // Create the third child process
    if ((pid3 = fork()) == 0) {
        // Third child process
        printf("Child 3 (PID: %d) is running.\n", getpid());
        sleep(3);
        printf("Child 3 (PID: %d) is done.\n", getpid());
        exit(EXIT_SUCCESS);
    } else if (pid3 < 0) {
        perror("Failed to fork Child 3");
        exit(EXIT_FAILURE);
    }

    // Parent process
    printf("Parent (PID: %d) is waiting for Child 2 (PID: %d).\n", getpid(), pid2);

    // Wait specifically for the second child
    int status;
    if (waitpid(pid2, &status, 0) == -1) {
        perror("Failed to wait for Child 2");
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
        printf("Child 2 exited with status %d.\n", WEXITSTATUS(status));
    } else {
        printf("Child 2 did not exit normally.\n");
    }

    // Wait for any remaining child processes
    while (wait(NULL) > 0);

    printf("Parent is done.\n");

    return 0;
}
