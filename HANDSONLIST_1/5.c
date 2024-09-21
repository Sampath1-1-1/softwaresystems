// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 5 of Hands On list1
//5.Write a program to create five new files with infinite loop. Execute the program in the background
// and check the file descriptor table at /proc/pid/fd.
#include <fcntl.h>   // for open() and O_CREAT
#include <unistd.h>  // for close() and sleep()
#include <stdio.h>   // for perror()
#include <stdlib.h>  // for exit()
#define Numfiles 5

int main() {
   
int fds[Numfiles];
    while (1) {
        for (int i = 0; i < Numfiles; i++) {
            // Create a unique filename
           char filename[25];//filename array  store the names of the files that the program will create
        snprintf(filename, sizeof(filename), "infinitefiles%d.txt",i);//it ensures that each file gets unique name
// int snprintf(char *str, size_t size, const char *format, ...);
// str: A pointer to the buffer where the formatted string will be stored.
// size: The size of the buffer (str). This is the maximum number of characters to write to the buffer, including the null terminator.
//snprintf ensures that the buffer does not overflow by specifying the maximum number of characters to write.
// format: A format string that specifies how to format the output. This is similar to the format string used in printf.
// ...: Additional arguments that will be formatted according to the format string.

            //  Open file with O_CREAT to create it if it doesn't exist
            fds[i] = open(filename, O_CREAT | O_WRONLY, 0644);
            if (fds[i] < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            // Close the file descriptor
            close(fds[i]);
        }

        // Sleep to reduce CPU usage
        sleep(1);
    }

    return 0;
}

