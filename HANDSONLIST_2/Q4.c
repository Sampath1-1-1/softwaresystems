// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 4 of Hands On list 2
//4. Write a program to measure how much time is taken to execute 100 getppid ( ) system call. Use time stamp counter.
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>

unsigned long long rdtsc() {//function will return a 64-bit unsigned integer
    unsigned long long tsc;
    __asm__ __volatile__ ("rdtsc" : "=A" (tsc));
    //__volatile__: This keyword tells the compiler not to optimize this assembly code away.
    //rdtsc instruction reads the current value of the processor's Time Stamp Counter (TSC).
    //moving the TSC value from the rax register (or the equivalent) into the tsc variable.
    return tsc;
}

int main() {
    unsigned long long start, end;
    // Start time (TSC)
    start = rdtsc();
    // Execute getppid() 100 times
    for(int i = 0; i < 100; i++) {
        getppid();
    }
    
    // End time (TSC)
    end = rdtsc();
    
    // Calculate and print the elapsed time (in clock cycles)
    printf("Time taken for 100 getppid() calls: %llu clock cycles\n", end - start);
    
    return 0;
}

