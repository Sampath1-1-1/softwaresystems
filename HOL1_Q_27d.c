// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 27 of Hands On list1
//27.Write a program to execute ls -Rl by the following system calls
// a. execl  b. execlp  c. execle d. execv    e. execvp
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    char *const args[] = {"/bin/ls", "-Rl", NULL};
    // declares args as an array of char* (character pointers), 
    execv(args[0], args);//argv[0] means it refers the first argument which is /bin/ls

    // If execv fails, it returns -1
    perror("execv failed");
    return 1;
}
