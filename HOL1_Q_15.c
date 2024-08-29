// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 15 of Hands On list1
//15.Write a program to display the environmental variable of the user (use environ).
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
char **environ; 
int main(void) {
    char **env = environ;  // Point to the environment variables

    // Loop through each environment variable and print it
    while (*env) {
        printf("%s\n", *env);
        env++;
    }

    return 0;
}
