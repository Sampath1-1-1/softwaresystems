// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 16 of Hands On list1
// 16.Write a program to perform mandatory locking.
// a. Implement write lock
// b. Implement read lock
#include <fcntl.h>  // For open() and flags
#include <unistd.h> // For close() and sleep()
#include <stdio.h>  // For perror()

#define NUM_FILES 5 // Number of files to create

int main() {
    int fds[NUM_FILES]; // Array to hold file descriptors

    // Open five new files in write-only mode
    for (int i = 0; i < NUM_FILES; i++) {
        // Construct file name
        char filename[20];
        snprintf(filename, sizeof(filename), "file%d.txt", i);

        // Open file with O_CREAT to create it if it doesn't exist
        fds[i] = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fds[i] == -1) {
            perror("open"); // Print an error message if open fails
            return 1;
        }
    }

    // Infinite loop to keep the file descriptors open
    while (1) {
        sleep(10); // Sleep for 10 seconds to avoid high CPU usage
    }

    // Close file descriptors (this will never be reached due to infinite loop)
    for (int i = 0; i < NUM_FILES; i++) {
        close(fds[i]);
    }

    return 0;
}

