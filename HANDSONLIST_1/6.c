// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 6 of Hands On list1
//6.Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
#include <unistd.h> // For read() and write()
#include <stdio.h>  // For perror()
#include <fcntl.h>  // For constants like O_RDONLY
    #define Buffersize 64
int main() {
    char buffer[Buffersize];
    ssize_t bytesRead;//it is datatype 
//     Positive Values: It represents the no  of bytes successfully read or written.EX:if read reads 100 bytes, ssize_t will hold the value 100.
// Negative Values: It can also represent error conditions. EX:if an error occurs during a read or write operation, these functions return -1. 
// This allows the program to detect errors and handle them appropriately.
// Read from standard input (file descriptor 0)
    bytesRead = read(0, buffer, Buffersize); //read can return a positive number (the count of bytes read), or -1 to indicate an error.
    //read(int fd, void *buf, size_t count);paremeters of read function
    // buf is the pointer to a buffer where the read data will be stored
    //buffersize represents the no of bytes we want to read
 // If read failed, exit with error code
    if (bytesRead < 0) {
        write(2, "Read error", 10);//10 specifies no of characters we are going to  written
        // write(int fd, const void *buf, size_t count);3 parameters
        // fd to specify where the data should be written
        // buf is pointing to buffer containing the data to be written
        //third parameter specifies no of characters should be written
       
    //     2 is the file descriptor for standard error (stderr).
    // 0 is for standard input (stdin).
    // 1 is for standard output (stdout).
    // 2 is for standard error (stderr).
        return 1;
    }

    // Write to standard output (file descriptor 1)
    if (write(1, buffer, bytesRead) < 0) {//bytesRead specifies no of bytes we read.we  will write same no of bytes as we read
        write(2, "Write error\n", 12);
        return 1;
    }
}