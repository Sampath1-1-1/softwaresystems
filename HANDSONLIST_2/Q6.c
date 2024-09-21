// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 6 of Hands On list 2
//6. Write a simple program to create three threads.
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <pthread.h>

// Function that will be executed by each thread
void* thread_function(void * arg) {
    //This allows the function to accept any type of data as an argument
    int thread_num = *((int*)arg); // Cast and dereference the argument to get the thread number
    printf("Hii from thread %d\n", thread_num);
    return NULL;
}

int main() {
    pthread_t threads[3]; // declares an array named threads that can hold three pthread_t identifiers.
    //pthread_t is a data type provided by the pthread library to represent a thread ID
    //It is used to uniquely identify a thread in a process
    int thread_args[3];   // Array to hold thread arguments (thread numbers)
    // Create three threads
    for(int i = 0; i < 3; i++) {
        thread_args[i] = i + 1; // Assign a thread number (1, 2, 3)
        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        //int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
//attr: A pointer to a pthread_attr_t structure that specifies thread attributes. If NULL, default attributes are used.
// 3rd argument pointer to the function that the thread will execute. This function must have the form void *(*start_routine)(void *).
//arg: A pointer to the argument that will be passed to the thread function.
        //pthread_create is a function  that creates a new thread.
    }

    // Wait for all threads to finish
    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
//pthread_join(threads[i], NULL);: Waits for each thread (from threads[0] to threads[2]) to finish. The NULL indicates that we don't 
// care about the thread's return value
    printf("All threads have finished execution.\n");

    return 0;
}

