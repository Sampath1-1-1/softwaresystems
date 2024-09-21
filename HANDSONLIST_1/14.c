// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 14 of Hands On list1
//14.Write a program to find the type of a file.
// a. Input should be taken from command line.
// b. program should be able to identify any type of a file.
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

    // struct stat {
    // dev_t     st_dev;     // Device ID
    // ino_t     st_ino;     // Inode number
    // mode_t    st_mode;    // File type and mode
    // nlink_t   st_nlink;   // Number of hard links
    // uid_t     st_uid;     // User ID of owner
    // gid_t     st_gid;     // Group ID of owner
    // dev_t     st_rdev;    // Device ID (if special file)
    // off_t     st_size;    // Size of file in bytes
    // time_t    st_atime;   // Time of last access
    // time_t    st_mtime;   // Time of last modification
    // time_t    st_ctime;   // Time of last status change
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        // This line is commonly used when a program expects arguments from the command line and the user has not provided the necessary arguments.
        //  It shows the user how they are supposed to run the program properly.
        // return 1;
    }

    struct stat file_stat;

    // Get file status
    if (stat(argv[1], &file_stat) != 0) {
        perror("Error getting file status");
        return 1;
    }

    // Check the type of file
    if (S_ISREG(file_stat.st_mode)) {//it will return true if the file is regular otherwise false
        printf("The file is a regular file.\n");
    } 
    else if (S_ISDIR(file_stat.st_mode)) {
        printf("The file is a directory.\n");
    } 
    else if (S_ISLNK(file_stat.st_mode)) {
        printf("The file is a symbolic link.\n");
    } 
    else if (S_ISCHR(file_stat.st_mode)) {
        printf("The file is a character device.\n");
    }
     else if (S_ISBLK(file_stat.st_mode)) {
        printf("The file is a block device.\n");//A type of device file in Unix-like operating systems that
        //  represents hardware devices capable of storing and transferring data in large, fixed-size blocks. example:hard disk,cd etc

    }
     else if (S_ISFIFO(file_stat.st_mode)) {
        printf("The file is a FIFO (named pipe).\n");
    }
     else if (S_ISSOCK(file_stat.st_mode)) {
        printf("The file is a socket.\n");
    }
     else {
        printf("The file type is unknown.\n");
    }

    return 0;
}

