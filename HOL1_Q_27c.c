// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 27 of Hands On list1
//27.Write a program to execute ls -Rl by the following system calls
// a. execl  b. execlp  c. execle d. execv    e. execvp
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    execle("/bin/ls", "ls" , NULL, NULL);
    //execle allows you to pass environment variables as an argument list.

    // If execle fails, it returns -1
    perror("execle failed");
    return 1;
}
