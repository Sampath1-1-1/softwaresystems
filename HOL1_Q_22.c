// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 22 of Hands On list1
//22.Write a program, open a file, call fork, and then write to the file by both the child as well as the parent processes. Check output of the file.
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
int main(){ 
     int fd = open("hello.txt", O_WRONLY | O_CREAT|O_APPEND, 0777);
    if (fd == -1) {
        perror("error in opening the file");
        return 1;
    }

    int pid = fork();
    if (pid<0)
    {
        /* code */
         perror("fork error");
        return 1;
    }

    if (pid == 0) {
        
        
        write(fd, "this is written by the child process.\n", 38);   // 38 size of the text else we can create a char string and give its name and size
    } else {
        wait(NULL);  // Wait for the child to finish
        write(fd, "this is written by the parent process.\n", 39);
    }

    close(fd);
    return 0;
} 
