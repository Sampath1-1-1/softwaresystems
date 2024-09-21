// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 30 of Hands On list 2
// 30. Write a program to create a shared memory.
// a. write some data to the shared memory
// b. attach with O_RDONLY and check whether you are able to overwrite.
// c. detach the shared memory
// d. remove the shared memory
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024  // Size of shared memory

int main() {
    key_t key;
    int shmid;
    char *data;

    // 1. Generate unique key for shared memory
    key = ftok("shmfile", 6);  // "shmfile" can be any existing file in the system

    // 2. Create shared memory segment
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); // 0666 -> read and write permissions
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    // 3. Attach to the shared memory for writing
    data = (char *) shmat(shmid, NULL, 0); // 0 for read and write mode
    if (data == (char *)(-1)) {
        perror("shmat is  failed");
        exit(1);
    }

    // 4. Write some data to shared memory
    printf("Writing data to shared memory.\n");
    strcpy(data, "Hii, Shared Memory");
    printf("Data written: %s\n", data);

    // 5. Detach the shared memory (write mode)
    if (shmdt(data) == -1) {
        perror("shmdt failed");
        exit(1);
    }

    // 6. Attach again with O_RDONLY (read-only mode)
    data = (char *) shmat(shmid, NULL, SHM_RDONLY); // SHM_RDONLY for read-only
    if (data == (char *)(-1)) {
        perror("shmat failed");
        exit(1);
    }

    // 7. Try to overwrite the data in read-only mode
    printf("Trying to overwrite data in read-only mode.\n");
    strcpy(data, "Overwrite Attempt"); // This will cause a segmentation fault because it's read-only

    // 8. Detach the shared memory (read-only mode)
    if (shmdt(data) == -1) {
        perror("shmdt failed");
        exit(1);
    }

    // 9. Remove the shared memory
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        exit(1);
    }

    printf("Shared memory is removed successfully.\n");
    return 0;
}
