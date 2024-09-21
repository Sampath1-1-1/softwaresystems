// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 17 of Hands On list1
// //17.Write a program to simulate online ticket reservation. Implement write lock
// Write a program to open a file, store a ticket number and exit. Write a separate program, to
// open the file, implement write lock, read the ticket number, increment the number and print
// the new ticket number then close the file.
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include <errno.h>
#include <stdlib.h>
int main() {
    struct flock lck;
    int ticketno;
    int fd = open("17.txt", O_RDWR | O_CREAT, 0666);  
    lck.l_type = F_RDLCK;    
    lck.l_whence = SEEK_SET;
    lck.l_start = 0;       
    lck.l_len = sizeof(ticketno);
    lck.l_pid = getpid();

    printf("Before entering the read lock\n");
    sleep(2);

    // Acquire the read lock (blocking if necessary)
    fcntl(fd, F_SETLKW, &lck);
     

    printf("Inside read lock\n");
// Read the current ticket number
    char buffer[100]={1};
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
     ticketno = atoi(buffer);//atoi (ASCII to integer).  //atoi converts ascii to int .here buffer is character array which stores zeroes as string .so to convert 
     //string to int we use atoi
    printf("Current ticket number: %d \n", ticketno);

    printf("Press enter to unlock\n");
    getchar();  
    lck.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lck);
      

    printf("Read lock released\n");

    // Close the file
    close(fd);

    return 0;
}

