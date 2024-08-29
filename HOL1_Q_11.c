// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 11 of Hands On list1
//11.Write a program to open a file, duplicate the file descriptor and append the file with both the
// descriptors and check whether the file is updated properly or not.
// a. use dup
// b. use dup2
// c. use fcntl
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
void write_to_file(int fd, const char *data) {
    // The write_to_file function is a utility function designed to handle writing data to a file descriptor.
    //  It takes two parameters: a file descriptor (fd) and a pointer to a string (data). 
    // This function attempts to write the provided string to the file associated with the given file descriptor
    //data points to data to be written
    if (write(fd, data, strlen(data)) == -1) {
        perror("write");
    }
}

int main() {
    int fd, dup_fd, dup2_fd;
    const char *msg1 = "This message is Written using original fd.\n";
    const char *msg2 = "This message is Written using dup() fd.\n";
    const char *msg3 = "this message is written using dup2() fd.\n";

    // Open the file
    fd = open("creatingfile.sh", O_WRONLY | O_CREAT | O_APPEND);//to open a file in append mode
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Duplicate file descriptor using dup()
    dup_fd = fcntl(fd,F_DUPFD);//F_DUPED is command similar to dup fun which duplicates the given fd
    //The duplicate fd will refer to the same file description (file, pipe, socket, etc.) as the original fd
    //  The new fd created by F_DUPFD will be the smallest available fd number that is greater than or equal to minfd.
    //The minfd parameter specifies the minimum value for the new file descriptor.
    if (dup_fd == -1) {
        perror("dup");
        close(fd);
        return 1;
    }

    // Duplicate file descriptor using dup2()
    dup2_fd = fcntl(fd,F_DUPFD); // Use file descriptor 3
    if (dup2_fd == -1) {
        perror("dup2");
        close(fd);
        close(dup_fd);
        return 1;
    }

    // Write to the file using original and duplicated file descriptors
    write_to_file(fd, msg1);
    write_to_file(dup_fd, msg2);
    write_to_file(dup2_fd, msg3);
    //fd: The file descriptor to which the message will be written.
//msg: The message string to be written.

    // Close all file descriptors
    close(fd);
    close(dup_fd);
    close(dup2_fd);

    printf("Data has been written to %s successfully.\n", "creating.sh");

    return 0;
}