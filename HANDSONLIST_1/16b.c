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
    //F_SETLKW tells the fcntl() system call to attempt to acquire this lock, and if the lock is not immediately available, to wait until it can be acquired.

    printf("inside the write \n");
    char *data = strdup("strdup funtion is used to duplicate the given string\n");
    //This line duplicates a string and assigns the duplicated string to the pointer data.

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
