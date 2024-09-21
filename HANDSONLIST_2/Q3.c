// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 3 of Hands On list 2
//3. Write a program to set (any one) system resource limit. Use setrlimit system call.
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;

    // Get the current limits for the number of open files
    getrlimit(RLIMIT_NOFILE, &limit);
    printf("Current soft limit: %ld\n", limit.rlim_cur);
    printf("Current hard limit: %ld\n", limit.rlim_max);

    // Set a new soft limit for the number of open files
    limit.rlim_cur = 512;  // Set the soft limit to 512
    if (setrlimit(RLIMIT_NOFILE, &limit) == 0) {
        printf("New soft limit set successfully.\n");
    } else {
        perror("Failed to set new limit");
    }

    // Get the updated limits for the number of open files
    getrlimit(RLIMIT_NOFILE, &limit);
    printf("Updated soft limit: %ld\n", limit.rlim_cur);
    printf("Updated hard limit: %ld\n", limit.rlim_max);

    return 0;
}
