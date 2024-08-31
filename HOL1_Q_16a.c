// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 16 of Hands On list1
// 16.Write a program to perform mandatory locking.
// a. Implement write lock
// b. Implement read lock
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
int main(){ 
    struct flock lck;
    int fd = open("16.txt",O_RDWR|O_APPEND|O_CREAT,0666);
    lck.l_type=F_WRLCK;
    lck.l_whence=SEEK_SET;
    lck.l_start = 0;
    lck.l_len = 0;
    lck.l_pid=getpid();
    printf("before enterig the write lock\n");
    fcntl(fd, F_SETLKW, &lck); 

    printf("inside the write \n");
    char *data = strdup("strdup funtion is used to duplicate the given string\n");

    if (write(fd, data, strlen(data) + 1) == -1) {
        perror("write error ");
        close(fd);
        return 1;

    }
    printf("press enter to unlock\n");
    getchar();

     lck.l_type = F_UNLCK;
     printf("write unlocked");
     fcntl(fd, F_SETLKW, &lck);
close(fd);
return 0; 
} 
