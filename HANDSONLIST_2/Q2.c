// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 2 of Hands On list 2
//2. Write a program to print the system resource limits. Use getrlimit system call.
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/resource.h> //getrlimit()

void print_limit(int resource, const char *resource_name) {
    //first argument tells the function which system resource limit to retrieve.
    //second argument holds the name of the resource.
    struct rlimit limit;
    //rlim_cur: The current (soft) limit for the resource.
// rlim_max: The maximum (hard) limit for the resource.

    // Get the resource limit
    if (getrlimit(resource, &limit) == 0) {
        // system call retrieves the resource limits for the specified resource
        printf("%s: \n", resource_name);
        printf("  Soft limit: %ld\n", (long)limit.rlim_cur);  // Current soft limit
        printf("  Hard limit: %ld\n\n", (long)limit.rlim_max);  // Maximum allowed limit
    } else {
        perror("getrlimit");  // Handle errors
    }
}

int main() {
    // Print limits for the stack size
    print_limit(RLIMIT_STACK, "Stack size");//stack size is returned in bytes
    //RLIMIT_STACK, a constant that refers to the stack size resource limit.

    // Print limits for the number of open files
    print_limit(RLIMIT_NOFILE, "Number of open files");
//RLIMIT_NOFILE refers to the resource limit for the maximum number of open file descriptors.
    // Print limits for CPU time
    print_limit(RLIMIT_CPU, "CPU time");
//RLIMIT_CPU refers to the resource limit for CPU time (how much processing time the program is allowed to use).
    return 0;
}
// Stack size: 
//   Soft limit: 8388608 (8MB)
//   Hard limit: -1.The value -1 means that there is no upper hard limit (it's unlimited)
// Number of open files: 
//   Soft limit: 1048576   no of files a program can open simultaneously
//   Hard limit: 1048576

// CPU time: 
//   Soft limit: -1
//   Hard limit: -1 .The program can use the CPU for as long as needed.
//Soft Limit: The soft limit is the value that can be enforced or modified by the program during its execution.
//Regular users and programs can't raise their resource limits above the hard limit, but privileged users (like root) can.