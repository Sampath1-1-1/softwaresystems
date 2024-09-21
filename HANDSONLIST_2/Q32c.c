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
#include <sys/sem.h>
#include<fcntl.h>
#include <unistd.h>
#include<error.h>
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
    int semid;
    key_t key = ftok("creatingfile.sh", 6); // Counting semaphore key

    // Get the existing counting semaphore
    semid = semget(key, 1, 0666);
    if (semid == -1) {
        perror("semget");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        P(semid); // Wait (enter critical section)
        printf("Using pseudo resource: %d\n", i + 1);
        sleep(1); // Simulate time taken to use resource
        V(semid); // Signal (exit critical section)
    }

    return 0;
}