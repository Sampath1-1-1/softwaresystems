// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 2 of Hands On list1
//2.Write a simple program to execute in an infinite loop at the background. Go to /proc directory and
// identify all the process related information in the corresponding proc directory
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h> //pid_t is datatype which represents process id;
#include <unistd.h> // For sleep()

int main() {
    // Print a message to indicate the start of the program
    printf("Program started. Running an infinite loop...\n");

    // Infinite loop
    while (1) {
        printf("Loop iteration...\n");
        sleep(5); // Sleep for 5 seconds
        
    }

    return 0; // This line will never be reached
}
//ctrl +c kills the process
