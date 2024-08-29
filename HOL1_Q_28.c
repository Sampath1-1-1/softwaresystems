// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 28 of Hands On list1
//28.Write a program to get maximum and minimum real time priority.
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
int main() {
    // Obtain maximum and minimum real-time priorities for SCHED_FIFO
    int max_priority_fifo = sched_get_priority_max(SCHED_FIFO);//It retrieves the maximum priority value for the SCHED_FIFO scheduling policy.


    int min_priority_fifo = sched_get_priority_min(SCHED_FIFO);

    // Check for errors
    if (max_priority_fifo == -1 || min_priority_fifo == -1) {
        perror("sched_get_priority_max or sched_get_priority_min failed");
        return EXIT_FAILURE;
    }

    // Print the results
    printf("Maximum real-time priority (SCHED_FIFO): %d\n", max_priority_fifo);
    printf("Minimum real-time priority (SCHED_FIFO): %d\n", min_priority_fifo);

    // Optionally obtain for SCHED_RR as well
    int max_priority_rr = sched_get_priority_max(SCHED_RR);
    int min_priority_rr = sched_get_priority_min(SCHED_RR);

    if (max_priority_rr == -1 || min_priority_rr == -1) {
        perror("sched_get_priority_max or sched_get_priority_min failed for SCHED_RR");
        return EXIT_FAILURE;
    }

    printf("Maximum real-time priority (SCHED_RR): %d\n", max_priority_rr);
    printf("Minimum real-time priority (SCHED_RR): %d\n", min_priority_rr);

    return EXIT_SUCCESS;
}

