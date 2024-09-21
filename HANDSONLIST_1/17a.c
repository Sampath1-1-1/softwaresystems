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
#include <string.h>
// write ticket
int main() {
    struct flock lck;
    // Open the file for reading and writing, create it if it doesn't exist
  int   fd = open("17.txt", O_RDWR | O_CREAT, 0666);
   
    lck.l_type = F_WRLCK;   
    lck.l_whence = SEEK_SET;
    lck.l_start = 0;        
    lck.l_len = 0;          
    lck.l_pid = getpid();
    printf("Before entering the write lock\n");
    sleep(2); 

    fcntl(fd, F_SETLKW, &lck);
    printf("Inside write lock\n");

    // Read the current ticket number
    char buffer[150] = {1};
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("read");
        close(fd);
        return 1;
    }

   
    int ticketno = atoi(buffer);  
    ticketno += 1;             

    snprintf(buffer, sizeof(buffer), "%d\n", ticketno); // back to string 

    // Reset the file offset to the beginning before writing
    lseek(fd, 0, SEEK_SET);

    // Write the new ticket number back to the file
    ssize_t bytesWritten = write(fd, buffer, strlen(buffer));
    
    printf("New ticket number: %d\n ", ticketno);

    printf("Press enter to unlock\n");
    getchar(); 

    
    lck.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lck);
     

    printf("Write lock released\n");

    // Close the file
    close(fd);

    return 0;
}