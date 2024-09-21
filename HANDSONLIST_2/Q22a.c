// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 22 of Hands On list 2
// 22. Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    int fd;
    mkfifo("my_fifo3", 0666);
    fd = open("my_fifo3", O_WRONLY);
    sleep(2); // Sleep for 2 seconds to simulate some delay
    write(fd, "Hello from writer process!\n", 27);
    close(fd);
    return 0;
}