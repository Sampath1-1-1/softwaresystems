// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 7 of Hands On list1
//7.Write a program to copy file1 into file2 ($cp file1 file2).
#include <unistd.h>  // For read(), write(), close()
#include <fcntl.h>   // For open(), O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <stdlib.h>  // For exit()
#include <stdio.h>   // For perror()

#define Buffersize 1024

int main(int argc, char *argv[]) {
    if (argc != 3) { fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]); exit(1); }

    int src = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (src < 0 || dest < 0) { perror("Error opening file"); exit(1); }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(src, buffer, BUFFER_SIZE)) > 0) write(dest, buffer, bytesRead);

    close(src);
    close(dest);

    return 0;
}
