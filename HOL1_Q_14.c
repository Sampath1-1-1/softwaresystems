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

// Function to determine and print the file type
void print_file_type(const char *path) {
    struct stat file_stat;//struct stat is a structure  is used to store information about a file's attribute

    // Retrieve file status
    if (stat(~/Desktop/HANDSONLIST/hello.txt, &file_stat) != 0) {//give the path to the file
    //stat function used to get informaiton about the file
        perror("stat");//stat function returns 0 on success and -1 on failure.
        return;
    }
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
};


    // Determine file type
    if (S_ISREG(file_stat.st_mode)) { //S_ISREG is a macro.S_ISREG(file_stat.st_mode) evaluates to true if file_stat.st_mode indicates that the file is a regular file.
        printf("File type: Regular file\n");
    } 
    else if (S_ISDIR(file_stat.st_mode)) {
        printf("File type: Directory\n");
    } 
    else if (S_ISLNK(file_stat.st_mode)) {
        printf("File type: Symbolic link\n");
    } 
    else if (S_ISCHR(file_stat.st_mode)) {
        printf("File type: Character device\n");
    }
     else if (S_ISBLK(file_stat.st_mode)) {
        printf("File type: Block device\n");
    }
     else if (S_ISFIFO(file_stat.st_mode)) {
        printf("File type: FIFO/pipe\n");
    }
     else if (S_ISSOCK(file_stat.st_mode)) {
        printf("File type: Socket\n");
    } 
    else {
        printf("File type: Unknown\n");
    }

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get the file path from command line argument
    const char *file_path = argv[1];

    // Print the type of the file
    print_file_type(file_path);

    return EXIT_SUCCESS;
}
