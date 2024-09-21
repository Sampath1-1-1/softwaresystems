// Rollno:MT2024132
////Name:Sampathkumar T
// Question No 4 of Hands On list1
//4.Write a program to open an existing file with read write mode. Try O_EXCL flag also.
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>//For error codes.
#include <string.h>//For the strerror function to get human-readable error messages.
int main() {
   // const char *filename = "example.txt";
   //int fd=open(filename,O_EXCL | O_RDWR, 0644));
   //EITHER WE CAN USE 9,10  OR ONLY 15
    // Open the file with read-write permissions
    int  fd = open("hello.txt",O_CREAT | O_EXCL | O_RDWR, 0644);
//O_EXCL is specifically for creating new files and avoiding overwriting existing ones
//O_CREAT is used  to create a new file if it does not already exist. If the file already exists, the O_CREAT flag has no effect on the existing file.
    if (fd == -1) { //open function returns -1 when file opening is failed
        // Check if the error is due to the file not existing
        if (errno == ENOENT) { //ENOENT means file doesnot exist.when we use O-CREAT in open function then this line is not required
            printf("File does not exist.\n");
        } else {
            printf("Error opening file: %s\n", strerror(errno)); //strerror(errno) converts the error code in errno into a human-readable string that describes the error
        }
        return 1;
    }

    printf("File '%s' opened successfully with file descriptor %d.\n", "hello.txt", fd);
    close(fd);

    return 0;
// }
// O_EXCL as a "safety lock" that you use when creating a file. It ensures that you don't accidentally overwrite an existing file. 
// If the file already exists, the operation stops and warns you instead of proceeding.
//O_EXCEL is always used with O_CREAT .we cannot use alone
//The O_EXCL flag is designed to work with O_CREAT to ensure that a new file is created, and if the file already exists, the operation fails