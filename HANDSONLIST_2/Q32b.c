// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 31 of Hands On list 2
// 32. Write a program to implement semaphore to protect any critical section.
// a. rewrite the ticket number creation program using semaphore
// b. protect shared memory from concurrent write access
// c. protect multiple pseudo resources ( may be two) using counting semaphore
// d. remove the created semaphore
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include<fcntl.h>
#include <string.h>
#include <unistd.h>

union semun {
     int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void P(int semid) {
    struct sembuf sb = {0, -1, 0}; // Wait (decrement)
    semop(semid, &sb, 1);
}

void V(int semid) {
    struct sembuf sb = {0, 1, 0}; // Signal (increment)
    semop(semid, &sb, 1);
}

int main(void) {
    int shmid;
    key_t key = ftok("creatingfile.sh", 6); // Shared memory key
    int semid = semget(ftok("creatingfile.sh", 6), 1, 0666); // Binary semaphore key

    // Create shared memory
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *shmaddr = shmat(shmid, NULL, 0);

    for (int i = 0; i < 5; i++) {
        P(semid); // Wait (enter critical section)
        snprintf(shmaddr, 1024, "Data is written: %d", i + 1);
        printf("Writing to shared memory which created: %s\n", shmaddr);
        sleep(1); // Simulate time taken to write
        V(semid); // Signal (exit critical section)
    }

    shmdt(shmaddr); // Detach shared memory
    return 0;
}
