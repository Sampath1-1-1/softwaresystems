// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 19 of Hands On list 2
// 19. Create a FIFO file by
// a. mknod command
// b. mkfifo command
// c. use strace command to find out, which command (mknod or mkfifo) is better.
// c. mknod system call
// d. mkfifo library function
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main() {
    // c. Create FIFO using mknod system call
    if (mknod("my_fifo_mknod", S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        return 1;
    }
     else {
        printf("FIFO created using mknod function call.\n");
    }

    // d. Create FIFO using mkfifo library function
    if (mkfifo("my_fifo_mkfifo", 0666) == -1) {
        perror("mkfifo");
        return 1;
    } else {
        printf("FIFO created using mkfifo function call.\n");
    }

    return 0;
}