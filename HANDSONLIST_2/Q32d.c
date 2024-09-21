// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 31 of Hands On list 2
// 32. Write a program to implement semaphore to protect any critical section.
// a. rewrite the ticket number creation program using semaphore
// b. protect shared memory from concurrent write access
// c. protect multiple pseudo resources ( may be two) using counting semaphore
// d. remove the created semaphore
#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(void) {
    int semid;
    key_t key = ftok("creatingfile.sh", 6); // Binary semaphore key

    // Get the existing semaphore
    semid = semget(key, 1, 0666);
    if (semid == -1) {
        perror("semget");
        return 1;
    }

    // Remove the semaphore
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        return 1;
    }

    printf("Semaphore removed successfully.\n");
    return 0;
}
