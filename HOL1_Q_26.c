// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 26 of Hands On list1
//26.Write a program to execute an executable program.
// a. use some executable program
// b. pass some input to an executable program. (for example execute an executable of $./a.out name)
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Define the command and arguments
    char *const args[] = {"./p2", "sampthkumar", NULL};
    
    // Execute the command with arguments
    execv(args[0], args);//system call used to execute the program 

    // If execv returns, it must have failed
    perror("execv failed");
    return 1;
    // Define the command to execute
    const char *program = "./p2 sampathkumar is fine";
    
    // Use popen to execute the command
    FILE *fp = popen(program, "r");
    //The popen function is used to execute a command (./p2 in this case) and open a pipe to read its output.
    //popen() returns a FILE * pointer that can be used to read the output of the command (when mode is "r") 
    if (fp == NULL) {
        perror("popen failed");
        return 1;
    }

    // Read the output from the command
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        //fgets() is  used to read a line of text from a file stream
        //fp is FILE * pointer to the file stream from which to read. here it points to the pipe opened by popen().
        printf("Output: %s", buffer);
    }

    // Close the pipe
    if (pclose(fp) == -1) {
        perror("pclose failed");
        return 1;
    }

    return 0;
}

