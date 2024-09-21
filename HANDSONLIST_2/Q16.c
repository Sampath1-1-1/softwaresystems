// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 16 of Hands On list 2
// 16. Write a program to send and receive data from parent to child vice versa. Use two way
// communication.
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int parent_to_child[2]; // Pipe for communication from parent to child
    int child_to_parent[2]; // Pipe for communication from child to parent
    pid_t pid;
    char parent_message[] = "Hii from parent";
    char child_message[] = "Hii from child";
    char buffer[100];

    // Create the pipes
    pipe(parent_to_child);  // Pipe for parent to child communication
    pipe(child_to_parent);  // Pipe for child to parent communication

    // Fork a new process
    pid = fork();

    if (pid == 0) {
        // Child process

        // Close unnecessary ends
        close(parent_to_child[1]);  // Close the write end of the parent-to-child pipe
        close(child_to_parent[0]);  // Close the read end of the child-to-parent pipe
        //it's best practice to close unused ends to avoid resource leaks, prevent deadlocks, and make sure communication behaves as expected.

        // Read message from parent
        read(parent_to_child[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Send message to parent
        write(child_to_parent[1], child_message, strlen(child_message) + 1);

        // Close the pipe ends used by the child
        close(parent_to_child[0]);
        close(child_to_parent[1]);
    } else {
        // Parent process

        // Close unnecessary ends
        close(parent_to_child[0]);  // Close the read end of the parent-to-child pipe
        close(child_to_parent[1]);  // Close the write end of the child-to-parent pipe

        // Send message to child
        write(parent_to_child[1], parent_message, strlen(parent_message) + 1);

        // Read message from child
        read(child_to_parent[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        // Close the pipe ends used by the parent
        close(parent_to_child[1]);
        close(child_to_parent[0]);
    }

    return 0;
}
