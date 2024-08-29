// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 9 of Hands On list1
//9.Write a program to print the following information about a given file.
// a. inode
// b. number of hard links
// c. uid
// d. gid
// e. size
// f. block size
// g. number of blocks
// h. time of last access
// i. time of last modification
// j. time of last change

#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

// Function to print file information
int main(){
    struct stat file_stat;
// struct stat structure is used to store information about a file, such as its size, type, and permissions
    // Get file information
    if (stat("creatingfile.sh", &file_stat) != 0) { //stat function is used to obtain information about file.it returns 0 on success and -1 on failure
    //stat(path,buffer)path: A pointer to a string that specifies the file or directory path you want to query.
    //2.buffer to store file information
        perror("Error retrieving file information");
        exit(EXIT_FAILURE);
    }

    // Print file attributes
    printf("Inode: %ld\n", (long)file_stat.st_ino);
    printf("Number of hard links: %ld\n", (long)file_stat.st_nlink);
    printf("UID: %u\n", file_stat.st_uid);
    printf("GID: %u\n", file_stat.st_gid);
    printf("Size: %ld bytes\n", (long)file_stat.st_size);
    printf("Block size: %ld bytes\n", (long)file_stat.st_blksize);
    printf("Number of blocks: %ld\n", (long)file_stat.st_blocks);

    // Print times
    printf("Last access time: %s", ctime(&file_stat.st_atime));
    printf("Last modification time: %s", ctime(&file_stat.st_mtime));
    printf("Last status change time: %s", ctime(&file_stat.st_ctime));
}

