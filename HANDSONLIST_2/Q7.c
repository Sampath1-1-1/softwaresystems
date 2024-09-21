// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 7 of Hands On list 2
//7. Write a simple program to print the created thread ids.
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
    // Print the thread ID of the current thread
    printf("Thread ID: %lu\n", pthread_self());  //pthread_self() fun returns the thread ID of the calling thread.
    return NULL;
}

int main() {
    pthread_t threads[3];  // Array to hold thread IDs
    int i;

    // Create 3 threads
    for(i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    // Wait for all threads to finish
    for(i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
