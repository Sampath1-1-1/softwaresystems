// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 7 of Hands On list1
//7.Write a program to copy file1 into file2 ($cp file1 file2).
#include <unistd.h>  // For read(), write(), close()
#include <fcntl.h>   // For open(), O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <stdlib.h>  // For exit()
#include <stdio.h>   // For perror()

#define Buffersize 1024//size 1kb
//The name of the program itself (always the first argument, argv[0]).
// A source file name (the second argument, argv[1]).
// A destination file name (the third argument, argv[2]).

int main(int argc, char *argv[]) {
    //argc is a count of how many arguments were given to the program.it stands for argument count
    //argv [] stands for argument vector
    if (argc != 3)
     {
         fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
         //stderr: The file stream where the output will be directed. stderr is typically used for error messages
        //   and diagnostic output. It is separate from stdout, which is used for regular program output.
         //It works similarly to printf, but instead of printing to the standard output (stdout), 
        //  it allows you to specify a file stream to which the formatted output will be written.
          exit(1);
          }

    int src = open(argv[1], O_RDONLY);
    //open function return the fd and stored in variable src
    //argv[1] refers to the first command-line argument provided to the program, which should be the name of the source file.
    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (src < 0 || dest < 0)
     { 
         perror("Error opening file"); 
          exit(1);
           }

    char buffer[Buffersize];
    ssize_t bytesRead;
    while ((bytesRead = read(src, buffer, Buffersize)) > 0) 
    write(dest, buffer, bytesRead);
    //write is a system call that writes data to a file descriptor (dest).
// It writes bytesRead bytes from the buffer to the destination file (dest)

//reads data from a file descriptor (src).
//It reads up to Buffersize bytes from the source file (src) into the buffer.
//The read function returns the number of bytes actually read, or 0 if the end of the file has been reached, or -1 if an error occurs.
    close(src);
    close(dest);

    return 0;
}

//./filecopy hello.txt file.txt 
//first argument is filename
//2nd argument is source file
//3 rd argument is destination file