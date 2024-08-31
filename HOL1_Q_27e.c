// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 27 of Hands On list1
//27.Write a program to execute ls -Rl by the following system calls
// a. execl  b. execlp  c. execle d. execv    e. execvp
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    char *const args[] = {"ls", "-Rl", NULL};
    execvp(args[0], args);

    // If execvp fails, it returns -1
    perror("execvp failed");
    return 1;
}
