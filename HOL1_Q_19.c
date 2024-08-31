// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 19 of Hands On list1
//19.Write a program to find out time taken to execute getpid system call. Use time stamp counter.
#include<fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

// Function to get the current Time Stamp Counter value
static inline uint64_t read_tsc() {
    //uint64_t is the return type of the function, which is a 64-bit unsigned integer.
    //read_tsc function is designed to read the Time Stamp Counter (TSC) of the CPU. 
    //he TSC is a 64-bit counter that counts the number of CPU cycles since the last reset.
    //TSC is a 64-bit counter that increments with each clock cycle of the CPU.


    uint64_t tsc;//unsigned 32-bit integer
//     low: Typically stands for "low". It stores the lower 32 bits of the 64-bit TSC value.
// high: Typically stands for "high". It stores the upper 32 bits of the 64-bit TSC value.
    // Assembly instruction to read TSC


    __asm__ volatile("rdtsc" : "=A" (tsc)); // Use "=A" to store the 64-bit result in tsc
    // //    "=A": This constraint tells the compiler to use the RAX register.
    //     RAX (64-bit) combines the EAX (lower 32 bits) and EDX (upper 32 bits) registers.
    //     RAX is a 64-bit register that can hold the full result of rdtsc.
    // (tsc):  tsc is a uint64_t variable that will receive the 64-bit TSC value.
    return tsc;

    //__asm__: This keyword allows you to write inline assembly code directly within your C program. 
    //It's a way to insert low-level assembly instructions into C code.
    //rdtsc:it is x86 assembly instruction reads the Time Stamp Counter (TSC). It provides a 64-bit result:
//     The lower 32 bits of the TSC value are placed in the EAX register.
// The upper 32 bits are placed in the EDX register.
//"=a": This tells the compiler that the EAX register (where the lower 32 bits of the TSC are stored) should be moved into the low variable.
    //return ((uint64_t)high << 32) | low;
    //(uint64_t)high << 32

//     (uint64_t)high: typecasts the high variable (which is a 32-bit integer) to a 64-bit integer type. 
//     << 32: Shifts the high value 32 bits to the left. This moves the upper 32 bits to the most significant position in the 64-bit result.
// | low
//     |: The bitwise OR operation combines the shifted high value with the low value.
//         The high value (after shifting) occupies the upper 32 bits.
//         The low value occupies the lower 32 bits.
//     Combining these with the bitwise OR operation produces the full 64-bit Time Stamp Counter value.
}

int main() {
    // Variables to hold the start and end TSC values
    uint64_t start, end;
    
    // Measure time before calling getpid
    start = read_tsc();
    
    // Call getpid
    pid_t pid = getpid();
    //pid_t is a data type used to represent process IDs
    
    // Measure time after calling getpid
    end = read_tsc();
    
    // Calculate the time taken
    uint64_t time_taken = end - start;
    
    // Print the PID and time taken
    printf("PID: %d\n", pid);
    printf("Time taken to execute getpid: %lu TSC cycles\n", time_taken);
    
    return 0;
}
