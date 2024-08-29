// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 12 of Hands On list1
//12.Write a program to find out the opening mode of a file. Use fcntl
#include <fcntl.h>  // For open, fcntl, O_RDONLY, etc.
#include <unistd.h> // For close
#include <stdio.h>  // For printf
#include <errno.h>  // For errno and perror

int main() {
    const char *filename = "12_openingmode.txt";
    int flags;

    // Open the file
   int fd = open(filename, O_RDONLY); // Open in read-only mode
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Get the file descriptor flags
    flags = fcntl(fd, F_GETFL);//The F_GETFL command is used to retrieve the current file status flags associated with a file descriptor.
    // it returns an integer where each bit represents a different file status flag or access mode like readonly ...
    if (flags < 0) {
        perror("fcntl");
        close(fd);
        return 1;
    }

    // Determine and print the file opening mode
    printf("File descriptor flags for '%s':\n", filename);
    if (flags & O_RDONLY) { //in flags if the readonly bit set to 1 then condition is true 
        printf("  Read-only mode\n");
    }
    if (flags & O_WRONLY) {
        printf("  Write-only mode\n");
    }
    if (flags & O_RDWR) {
        printf("  Read-write mode\n");
    }
    if (flags & O_APPEND) {
        printf("  Append mode\n");
    }
    if (flags & O_CREAT) {
        printf("  Create mode\n");
    }
    if (flags & O_TRUNC) {
        printf("  Truncate mode\n");//The file is cleared when opened.
    }
    if (flags & O_EXCL) {
        printf("  Exclusive mode\n");
    }

    // Close the file descriptor
    close(fd);

    return 0;
}
