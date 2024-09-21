// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 12 of Hands On list 2
//12. Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// int main() {
//     pid_t pid = fork();  // Create a new process

//     if (pid < 0) {
//         // Fork failed
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (pid == 0) {
//         // Child process
//         printf("Child process: PID=%d, Parent PID=%d\n", getpid(), getppid());

//         // Sleep for a while to ensure the parent process terminates
//         sleep(2);

//         // Send SIGKILL signal to the parent process
//         printf("Child process: Sending SIGKILL to parent PID=%d\n", getppid());
//         if (kill(getppid(), SIGKILL) == -1) {// Sends the SIGKILL signal to the parent process to forcefully terminate it.
//             perror("kill");
//             exit(EXIT_FAILURE);
//         }

//         // Wait for the parent to be terminated
//         sleep(2);

//         // Print a message indicating that the parent is terminated and child is now orphaned
//         printf("Child process: Parent is terminated. I am now an orphan.\n");

//         // Continue running to demonstrate orphan status
//         while (1) {
//             sleep(1);
//         }
//     } else {
//         // Parent process
//         printf("Parent process: PID=%d\n", getpid());

//         // Sleep for a while to let the child process execute
//         sleep(2);

//         // Parent process terminates before the child
//         printf("Parent process: Exiting...\n");
//         exit(EXIT_SUCCESS);
//     }

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process() {
    // Sleep for a short time to ensure the parent has terminated
    sleep(1);

    // Get the parent PID
    pid_t parent_pid = getppid();
    printf("Child (PID: %d) will send SIGKILL to Parent (PID: %d)\n", getpid(), parent_pid);
    
    // Send SIGKILL to the parent process
    kill(parent_pid, SIGKILL);
}

int main() {
    pid_t pid = fork(); // Create a new process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // In the child process
        child_process();
        exit(0); // Child exits after sending SIGKILL
    } else {
        // In the parent process
        printf("Parent (PID: %d) is running. Child (PID: %d) created.\n", getpid(), pid);
        
        // Parent process sleeps for 3 seconds
        sleep(3);
        printf("Parent (PID: %d) is terminating.\n", getpid());
        
        // Parent process will terminate here
        exit(0);
    }

    return 0;
}


