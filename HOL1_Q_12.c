// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 12 of Hands On list1
//12.Write a program to find out the opening mode of a file. Use fcntl
#include <fcntl.h>  // For open, fcntl, O_RDONLY, etc.
#include <unistd.h> // For close
#include <stdio.h>  // For printf
#include <errno.h>  // For errno and perror

int main() {
    const char *filename = "hello.txt";
    int flags;

    // Open the file
    int fd = open(filename, O_RDONLY | O_APPEND,0777); // Open in read-only mode
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Get the file descriptor flags
    flags = fcntl(fd, F_GETFL); // The F_GETFL command is used to retrieve the current file status flags associated with a file descriptor.
    //    // it returns an integer where each bit represents a different file status flag or access mode like readonly ...
    if (flags < 0) {
        perror("fcntl");
        close(fd);
        return 1;
    }

    // Determine and print the file opening mode
    printf("File descriptor flags for '%s':\n", filename);
    if ((flags &O_ACCMODE) == O_RDONLY) { // in flags if the readonly bit set to 1 then condition is true 
        printf("  Read-only mode\n");
        //O_ACCMODE It's used to extract the access mode from the file status flags.
    }
    if ((flags &O_ACCMODE) == O_WRONLY) {
        printf("  Write-only mode\n");
    }
    if ((flags &O_ACCMODE) == O_RDWR) {
        printf("  Read-write mode\n");
    }
    // Note: Flags like O_APPEND, O_CREAT, O_TRUNC, and O_EXCL might not be set if not used in the open() call
    if (flags & O_APPEND) {
        printf("  Append mode\n");
    }
    if (flags & O_CREAT) {
        printf("  Create mode\n");
    }
    if (flags & O_TRUNC) {
        printf("  Truncate mode\n");
    }
    if (flags & O_EXCL) {
        printf("  Exclusive mode\n");
    }

    // Close the file descriptor
    close(fd);

    return 0;
}
