// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 20 of Hands On list 2
//20. Write two programs so that both can communicate by FIFO -Use one way communication.
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char message[] ="Hello from writer";

   // const char *pi= "Hello from writer";//

    // Create a FIFO file (if it doesn't exist)
    mkfifo("myfifo", 0666);

    // Open the FIFO for writing
    int fd = open("myfifo", O_WRONLY);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    // Write message to the FIFO
    write(fd,message, sizeof(message));//data is stored in array so we use sizeof function.if data is stored using pointer then we should use strlen fun

    // Close the FIFO
    close(fd);

    printf("Message sent: %s\n", message);
    return 0;
}
