// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 10 of Hands On list1
//10.Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
// bytes (use lseek) and write again 10 bytes.
// a. check the return value of lseek
// b. open the file with od and check the empty spaces in between the data.
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILENAME "hello.txt"
#define DATA1 "1234567890"
#define DATA2 "ABCDEFGHIJ"
#define Bytes_to_write 10

int main() {
    off_t offset;//offset is the number of bytes to move the file pointer.

    // Open the file in read-write mode, create it if it doesn't exist, and set permissions to rw-rw-rw-
   int  fd = open(FILENAME, O_RDWR | O_CREAT, 0666);//O_CREAT flag is used to instruct the OS to create a file if it doesn't 
    //exist when opening a file. If the file already exists,
    // this flag does not affect the existing file— it simply allows the file to be opened for reading and/or writing.
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Write the first 10 bytes
    if (write(fd, DATA1, Bytes_to_write) != Bytes_to_write) {//write system call is used to write data from a buffer to a file descriptor
    // The write system call uses this file descriptor to determine where to send the data.
    //successful write system call returns no of bytes it actually written.
    //     Error or Partial Write: If write does not complete writing the requested number of bytes, 
    //it returns a number less than BYTES_TO_WRITE or -1 if an error occurred.


        perror("write");
        close(fd);
        return 1;
    }

    // Move the file pointer by 10 bytes
    offset = lseek(fd, Bytes_to_write, SEEK_END);//lseek is used to move the file pointer to a specific location within the file.
    //seek_cur gives current file position
    //lseek function  returns  the new position of the file pointer.
    // SEEK_SET is used to set the file offset relative to the beginning of the file.
//Error: Returns (off_t)-1 and sets errno to indicate the error.
    if (offset == (off_t)-1) {
        //It returns the new offset position on success or (off_t)-1 on failure.
        perror("lseek");
        close(fd);
        return 1;
    }

    // Write the next 10 bytes
    if (write(fd, DATA2, Bytes_to_write) != Bytes_to_write) {
        perror("write");
        close(fd);
        return 1;
    }

    // Close the file
    close(fd);

    printf("File operations completed successfully.\n");

    return 0;
}
// off_t lseek(int fd, off_t offset, int whence);
// offset: Number of bytes to move the file pointer.
// whence: The reference point from which to move the file pointer. This is where SEEK_SET, SEEK_CUR, and SEEK_END come into play/
//SEEK_END is used to set the file offset relative to the end of the file.