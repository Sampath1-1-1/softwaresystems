// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 3 of Hands On list1
//3.Write a program to create a file and print the file descriptor value. Use creat ( ) system call
#include <unistd.h> // For close() function
#include <stdio.h>  // For printf() and perror()
#include <stdlib.h> // For exit()
#include <fcntl.h>  // For creat() 
int main(){

    // Create a file with read/write permissions for the user (0644)
    //int creat(const char *pathname, mode_t mode);
 int fd = creat("myfile.txt", 0644);//variable fd is used to store the file descriptor returned by the creat() system call.
// we are creating file in current working directory file name is myfile.txt
//creat("/home/user/documents/example.txt"), the file will be created or opened at the exact location specified
    // Check if file creation was successful
    if (fd == -1) {
        // Print an error message and exit if creat() failed
        perror("Error creating file");
        //It prints a descriptive error message to the standard error output (stderr) based on the current value of the global variable errno
        // errno stores what error is occurred
        exit(EXIT_FAILURE);
    }
//     0 is the file descriptor for standard input (stdin).
//stdin is treated as a file. However, it’s a special type of file known as a "character device file," which reads from the keyboard or another input source.
//     1 is the file descriptor for standard output (stdout).
//     2 is the file descriptor for standard error (stderr).
// When a new file is created or opened, the system assigns the smallest available file descriptor that is greater than or equal to 3.

    // Print the file descriptor value
    printf("File descriptor: %d\n", fd);

    // Close the file descriptor
    close(fd);//It effectively deallocates the file descriptor, allowing it to be assigned to another file or resource in the future.
 return 0;
}
