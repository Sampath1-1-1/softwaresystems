// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 27 of Hands On list1
//27.Write a program to execute ls -Rl by the following system calls
// a. execl  b. execlp  c. execle d. execv    e. execvp
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    execlp("ls", "ls", "-Rl", NULL);
    //when you call execlp, it prepares the arguments and then invokes execve to execute the new program.
    //execlp is a function used to execute a file (command) within the current process.

    // If execlp fails, it returns -1
    perror("execlp failed");
    return 1;
}
// execl
// You must specify the program's path and each argument separately.
// It does not use the PATH environment variable, so you must provide the full or relative path to the program. 
// Function	Pass Arguments	 Path Resolution	                 Custom Environment
// execl	   List	             Full path required                  Inherited
// execlp	   List	             Uses PATH	                       Inherited
// execle	   List	              Full path required	             custom environment
// execv	   Array	        Full path required	                  Inherited
// execvp	   Array	        Uses PATH	                       Inherited