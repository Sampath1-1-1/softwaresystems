// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 8 of Hands On list 2
// 8. Write a separate program using signal system call to catch the following signals.
// a. SIGSEGV
// b. SIGINT
// c. SIGFPE
// d. SIGALRM (use alarm system call)
// e. SIGALRM (use setitimer system call)
// f. SIGVTALRM (use setitimer system call)
// g. SIGPROF (use setitimer system call)
// #define _GNU_SOURCE
// #include <stdio.h>
// #include <signal.h>
// #include <stdlib.h>
// #include <fenv.h>  // For floating-point exception handling

// // Signal handler for SIGFPE
// void handle_sigfpe(int signal) {
//     printf("Caught SIGFPE (Floating-point exception)\n");
//     exit(1);
// }

// int main() {
//     // Set up signal handler
//     signal(SIGFPE, handle_sigfpe);
// // SIGFPE stands for Signal Floating Point Exception

//   // Enable floating-point exception handling
//     feenableexcept(FE_DIVBYZERO); 
//     // Trigger SIGFPE by dividing by zero
//    float x = 1.0 / 0.0;  // This will cause a floating-point exception

//     return 0;
// }
//common causes of SIGFPE
//1.DIvision by zero
//2.Overflow-When a floating-point operation produces a result that exceeds the representable range of the floating-point type.


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// Signal handler for SIGFPE
void handler(int signum) {
    printf("Caught SIGFPE: Division by zero!\n");
   exit(0); 
}

int main() {
    // Register the signal handler for SIGFPE
    signal(SIGFPE, handler);

    int i, j;

    // Loop to demonstrate division by zero
    for (i = 0; i < 10; i++) {
        j = i / (i - 5); // This will cause SIGFPE when i == 5
        printf("Result of %d / (%d - 5) = %d\n", i, i, j);
    }

    printf("After for loop\n");
    return 0;
}

