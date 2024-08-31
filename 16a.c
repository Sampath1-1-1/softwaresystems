// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 16 of Hands On list1
// 16.Write a program to perform mandatory locking.
// a. Implement write lock
// b. Implement read lock
#include <fcntl.h>  // For open() and flags
#include <unistd.h> // For close() and sleep()
#include <stdio.h>  // For perror()
int main(){ 
    struct flock lck;
    // The structure typically contains the following members:

    // short l_type: Specifies the type of lock. It can be:
    //     F_RDLCK: Read lock. Multiple processes can have a read lock on the file simultaneously.
    //     F_WRLCK: Write lock. Only one process can have a write lock on the file, and it excludes other read or write locks.
    //     F_UNLCK: Unlock. This removes any locks previously placed on the file.
    // short l_whence: Specifies the reference point for l_start. It can be:
    //     SEEK_SET: The start of the file.
    //     SEEK_CUR: The current file offset.
    //     SEEK_END: The end of the file.
    // off_t l_start: The starting offset for the lock, relative to l_whence.
    // off_t l_len: The number of bytes to lock. If l_len is 0, it locks to the end of the file.
    // pid_t l_pid: The process ID of the process holding the lock. This is usually set by the system.
    //This structure is used to describe a file lock, which can be used to control access to a file in a multi-process environment.
    int fd = open("hello.txt",O_RDWR|O_CREAT,0666);
    lck.l_type=F_RDLCK;
    lck.l_whence=SEEK_SET;
    lck.l_start = 0;
    //0 indicates that the locking begins at the very first byte of the file.
    lck.l_len = 0;
    //l_len = 0: Lock from the starting point to the end of the file.

    //l_start = 10: The lock starts at byte 10 of the file.
// l_len = 50: The lock extends 50 bytes from the starting point (byte 10).

    lck.l_pid=getpid();
    printf("before entering the read lock\n");
    sleep(2); 

    fcntl(fd, F_SETLK, &lck); 
      //F_SETLKW is not used in read because it will 
                                 // not make other file to aquire the lock but in read we can have multiple read lock
                                 //&lck contains the details of the lock (start point, length, type). The F_SETLK command applies the lock immediately if possible.

    printf("inside read \n");

    printf("press enter to unlock\n");

    getchar();

     lck.l_type = F_UNLCK;
     printf("read unlocked\n");
     fcntl(fd, F_SETLK, &lck);

     close(fd);



 
 
return 0; 
} 