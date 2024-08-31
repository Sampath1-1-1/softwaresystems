// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 27 of Hands On list1
//27.Write a program to execute ls -Rl by the following system calls
// a. execl  b. execlp  c. execle d. execv    e. execvp
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    execl("/bin/ls", "ls", NULL);
    //This marks the end of the argument list. execl requires that the argument list ends with a NULL pointer.

    // If execl fails, it returns -1
    perror("execl failed");
    return 1;
}