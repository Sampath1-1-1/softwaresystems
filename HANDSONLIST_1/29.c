// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 29 of Hands On list1
//29.Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,SCHED_RR).
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int policy;
    struct sched_param param;//: This structure holds scheduling parameters for a process.
    int newpolicy = SCHED_FIFO; // Change to SCHED_RR to use Round-Robin policy

    // Get current scheduling policy
    policy = sched_getscheduler(0); // 0 refers to the current process
    //sched_getscheduler is a system call that retrieves the scheduling policy of a process.
    if (policy == -1) {
        perror("sched_getscheduler failed");
        return 1;
    }

    // Print current policy
    printf("Current scheduling policy: ");
    if (policy == SCHED_FIFO) printf("SCHED_FIFO\n");
    else if (policy == SCHED_RR) printf("SCHED_RR\n");
    else if (policy == SCHED_OTHER) printf("SCHED_OTHER\n");
    else printf("Unknown\n");

    // Set new scheduling policy
    param.sched_priority = 10; // Set a priority value (valid for SCHED_FIFO and SCHED_RR)
    if (sched_setscheduler(0, newpolicy, &param) == -1) {//0 refers to the current process
    //int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
//The sched_setscheduler function is called to change the scheduling policy and priority of the current process
//The  int policy parameter specifies the scheduling policy to apply
//param: A pointer to a sched_param structure that contains the priority value to be applied along with the policy.
        perror("sched_setscheduler failed");
        return 1;
    }

    // Get and print new scheduling policy
    policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("sched_getscheduler failed");
        return 1;
    }

    printf("New scheduling policy: ");
    if (policy == SCHED_FIFO) printf("SCHED_FIFO\n");
    else if (policy == SCHED_RR) printf("SCHED_RR\n");
    else if (policy == SCHED_OTHER) printf("SCHED_OTHER\n");
    else printf("Unknown\n");

    return 0;
}


