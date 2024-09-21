// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 18 of Hands On list1
//18.Write a program to perform Record locking.
// a. Implement write lock
// b. Implement read lock
// Create three records in a file. Whenever you access a particular record, first lock it then modify/access to avoid race condition.
#include <fcntl.h>    // For open(), fcntl(), and file locking
#include <unistd.h>   // For close(), lseek(), and sleep()
#include <stdio.h>    // For perror(), printf()
#include <string.h>   // For strlen()
#include <stdlib.h>   // For exit()
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


#define SIZE 20  // Size of each record including newline

// Function to acquire a lock
void acquire_lock(int fd, off_t offset, int len, int lock_type) {
    struct flock lock;
    lock.l_type = lock_type;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = len;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Failed to acquire lock");
        exit(EXIT_FAILURE);
    }
    printf("Locked\n");
}

// Function to release a lock
void release_lock(int fd, off_t offset, int len) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = len;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to release lock");
        exit(EXIT_FAILURE);
    }
    printf("Unlocked\n");
}

// Function to initialize the file with 3 records
void initialize_file(int fd) {
    const char *record1 = "Record 1          \n";
    const char *record2 = "Record 2          \n";
    const char *record3 = "Record 3          \n";

    // Write the records to the file
    if (write(fd, record1, SIZE) != SIZE ||
        write(fd, record2, SIZE) != SIZE ||
        write(fd, record3, SIZE) != SIZE) {
        perror("Failed to initialize file");
        exit(EXIT_FAILURE);
    }
}

// Function to read a record
void read_record(int fd, int index) {
    off_t offset = index * SIZE;
    //The byte position in the file where the record starts, calculated by multiplying the record index by the record size.
    acquire_lock(fd, offset, SIZE, F_RDLCK);  // Shared lock for read
//     offset: The starting byte position for the lock.
// SIZE: The number of bytes to lock, which should be the size of the record.
// F_RDLCK: Specifies a read lock (shared lock), allowing other processes to also acquire a read lock but not a write lock.

    lseek(fd, offset, SEEK_SET);
    char buffer[SIZE];
    if (read(fd, buffer, SIZE) != SIZE) {
        perror("Failed to read record");
        exit(EXIT_FAILURE);
    }
    printf("Read record %d: %s\n", index + 1, buffer);

    printf("Press Enter to unlock\n");
    getchar();  // Wait for user input
    getchar();  // Wait for Enter key

    release_lock(fd, offset, SIZE);
}

// Function to write a record
void write_record(int fd, int index, const char *new_record) {
    off_t offset = index * SIZE;
    acquire_lock(fd, offset, SIZE, F_WRLCK);  // Exclusive lock for write

    lseek(fd, offset, SEEK_SET);
    if (write(fd, new_record, SIZE) != SIZE) {
        perror("Failed to write record");
        exit(EXIT_FAILURE);
    }
    printf("Updated record %d: %s\n", index + 1, new_record);

    printf("Press Enter to unlock\n");
    getchar();  // Wait for user input
    getchar();  // Wait for Enter key

    release_lock(fd, offset, SIZE);
}

int main() {
    int fd = open("18.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Check if the file is empty
    off_t file_size = lseek(fd, 0, SEEK_END);
//     off_t lseek(int fd, off_t offset, int whence);
//  The number of bytes to move the file offset
//common method to determine the size of a file by setting the file offset to the end and reading the offset value, which represents the file size.
    if (file_size == 0) {
        // Initialize the file with 3 records if it's empty
        printf("File is empty. Initializing with 3 records...\n");
        initialize_file(fd);
    } else {
        printf("File already contains data. Skipping initialization.\n");
    }

    int block, mode;
    printf("Enter block number (1, 2, 3): ");
    scanf("%d", &block);

    switch (block) {
        case 1:
            printf("Enter mode (1 for read, 2 for write): ");
            scanf("%d", &mode);
            if (mode == 1) {
                read_record(fd, 0);
            } else if (mode == 2) {
                write_record(fd, 0, "New Record 1        \n");
            } else {
                printf("Invalid mode\n");
            }
            break;

        case 2:
            printf("Enter mode (1 for read, 2 for write): ");
            scanf("%d", &mode);
            if (mode == 1) {
                read_record(fd, 1);
            } else if (mode == 2) {
                write_record(fd, 1, "New Record 2        \n");
            } else {
                printf("Invalid mode\n");
            }
            break;

        case 3:
            printf("Enter mode (1 for read, 2 for write): ");
            scanf("%d", &mode);
            if (mode == 1) {
                read_record(fd, 2);
            } else if (mode == 2) {
                write_record(fd, 2, "New Record 3        \n");
            } else {
                printf("Invalid mode\n");
            }
            break;

        default:
            printf("Invalid block number\n");
            break;
    }

    close(fd);
    return 0;
}
