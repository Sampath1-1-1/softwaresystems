// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 20 of Hands On list1
//20.Find out the priority of your running program. Modify the priority with nice command.
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    // Display the current priority
    int old_priority = getpriority(PRIO_PROCESS, 0);//fun used to get priority 
    //getpriority(PRIO_PROCESS, 0) retrieves the priority of the current process.
    //PRIO_PROCESS specifies that you're interested in the priority of a  specific process.
    //PRIO_PROCESS: Used for individual processes.
// PRIO_PGRP: Used for process groups.
//0 refers to current process
// PRIO_USER: Used for all processes owned by a specific user.
    //priority  ranges from -19 to 20 and -19 is higest priority and 20 is lowest.in unix lowest the value the highest the priority
    if (old_priority == -1) {
        perror("getpriority"); // Print error if getpriority fails
        return 1;
    }
    printf("Current priority: %d\n", old_priority);

    // Increase the priority of the process
    int decrement = -19; // Priority adjustment (positive value lowers the priority)
    if (nice(decrement) == -1) {
        //The nice function is used to change the priority of the current process.
        perror("nice"); // Print error if nice fails
        return 1;
    }

    // Display the new priority
    int new_priority = getpriority(PRIO_PROCESS, 0);
    if (new_priority == -1) {
        perror("getpriority"); // Print error if getpriority fails
        return 1;
    }
    printf("New priority: %d\n", new_priority);

    // Simulate some work
    // printf("Simulating work...\n");
    // for (volatile int i = 0; i < 100000000; ++i); // Busy-wait loop to simulate work

    return 0;
}
