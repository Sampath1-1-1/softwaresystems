// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 31 of Hands On list 2
// 31. Write a program to create a semaphore and initialize value to the semaphore.
// a. create a binary semaphore
// b. create a counting semaphore
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    key_t key1, key2;
    int  binary_semid, counting_semid;

    // Generate unique keys for semaphores
    key1 = ftok("binsem", 15);  // Key for binary semaphore
    key2 = ftok("cntsem", 16);  // Key for counting semaphore

    // Create a binary semaphore (single semaphore set)
    binary_semid = semget(key1, 1, 0666 | IPC_CREAT); // 1 semaphore
    // int semget(key_t key, int nsems, int semflg);
//     key (key_t): A unique identifier (key) used to identify the semaphore set
//     nsems (int):The number of semaphores to create in the semaphore set.
// semflg (int):Flags that control the creation or access to the semaphore set
    if (  binary_semid== -1) {
        perror("semget (binary) failed");
        return 1;
    }

    // Initialize binary semaphore to 1 (unlocked state)
    if (semctl( binary_semid, 0, SETVAL, 1) == -1) {
        perror("semctl (binary init) failed");
        return 1;
    }
//     int semctl(int semid, int semnum, int cmd, ...);
// semid (int):This is the semaphore set identifier, which is obtained from a call to semget.

// semnum (int):This is the index of the specific semaphore within the semaphore set you want to operate on.
//  0 means the operation is being performed on the first semaphore in the set (since semaphore arrays are zero-indexed).

// cmd (int):This specifies the command to perform on the semaphore.
//     SETVAL is a command that sets the value of a semaphore to a specific value (provided as the 4th argument).

// arg (union semun or int):This is the optional argument that depends on the command.
//     For SETVAL, this argument is the integer value to set the semaphore to (in your case, 1).

    printf("Binary semaphore created and initialized to 1.\n");

    // Create a counting semaphore (single semaphore set)
   counting_semid = semget(key2, 1, 0666 | IPC_CREAT); // 1 semaphore
    if ( counting_semid== -1) {
        perror("semget (counting) failed");
        return 1;
    }

    // Initialize counting semaphore to 5 (5 available resources)
    if (semctl(counting_semid, 0, SETVAL, 5) == -1) {
        perror("semctl (counting init) failed");
        return 1;
    }

    printf("Counting semaphore created and initialized to 5.\n");

    return 0;
}
