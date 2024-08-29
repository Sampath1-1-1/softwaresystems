// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 5 of Hands On list1
//5.Write a program to create five new files with infinite loop. Execute the program in the background
// and check the file descriptor table at /proc/pid/fd.
#include <fcntl.h>  // For open() and flags
#include <unistd.h> // For close() and sleep()
#include <stdio.h>  // For perror()

#define Numfiles 5 // Number of files to create

int main() {
    int fds[Numfiles]; // Array to hold file descriptors

    // Open five new files in write-only mode
    for (int i = 0; i <Numfiles; i++) {
        // Construct file name
        char filename[25];//filename array  store the names of the files that the program will create
        snprintf(filename, sizeof(filename), "infinitefiles%d.txt",i);//it ensures that each file gets unique name

        // Open file with O_CREAT to create it if it doesn't exist
        fds[i] = open(filename, O_WRONLY | O_CREAT);
        //The filename specifies teh  name of the file to be opened or created. This is a string that specifies the path to the file.
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
    for (int i = 0; i < Numfiles; i++) {
        close(fds[i]);
    }

    return 0;
}

