// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 8 of Hands On list1
//8.Write a program to open a file in read only mode, read line by line and display each line as it is read.
// Close the file when end of file is reached.
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>

int main() {
    FILE *file;        // Pointer to the file
    //FILE Used to perform actual file I/O operations.
    char line[256];    // Buffer to hold each line

    // Open the file in read-only mode
    file = fopen("hello.txt", "r");//It is used to open a file and return a FILE* pointer for high-level file operations.
    //open function is for low level file operations and return file descriptor
    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read and display each line until end-of-file
    while (fgets(line, sizeof(line), file) != NULL) {//fgets() reads a line from the file.
    //If an error occurs or if the end of the file (EOF) is reached, fgets() returns NULL.
        printf("%s", line);//%s is used because we are printing the string
        //fgets() reads up to sizeof(line) - 1 characters from the file  and stores them in line. It stops reading when 
        //it encounters a newline character (\n), end-of-file (EOF), or the maximum number of characters minus one, whichever comes first.
    }

    // Close the file
    fclose(file);

    return 0;
}
