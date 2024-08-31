// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 15 of Hands On list1
//15.Write a program to display the environmental variable of the user (use environ).
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
extern  char **environ; 
//environ: This is a special list provided by the system. It contains all the environment variables for your program.
//This declaration tells the compiler that environ exists somewhere and will be linked during the linking stage of compilation.
int main(void) {
    char **env = environ;  // Point to the environment variables
    //env point to the same list of environment variables that environ points to.

    // Loop through each environment variable and print it
    while (*env) {//gets the current environment variable string from the list
        //The loop will keep going until it reaches the end of the list of environment variables.
        printf("%s\n", *env);
        env++;
    }

    return 0;
}
