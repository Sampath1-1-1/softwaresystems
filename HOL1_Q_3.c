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
    int fd = creat("myfile.txt", 0644);//variable fd is used to store the file descriptor returned by the creat() system call.
// we are creating file in current working directory file name is myfile.txt
//creat("/home/user/documents/example.txt"), the file will be created or opened at the exact location specified
    // Check if file creation was successful
    if (fd == -1) {
        // Print an error message and exit if creat() failed
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    // Print the file descriptor value
    printf("File descriptor: %d\n", fd);

    // Close the file descriptor
    close(fd);//t ensures that any buffered data is flushed and the file is properly closed.
 return 0;
}
