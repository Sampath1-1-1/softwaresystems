// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 20 of Hands On list 2
//20. Write two programs so that both can communicate by FIFO -Use one way communication.
// reader.c
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char buffer[100];

    // Create a FIFO file (if it doesn't exist)
   // mkfifo("myfifo", 0666);

    // Open the FIFO for reading
    int fd = open("myfifo", O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    // Read message from the FIFO
    read(fd, buffer, sizeof(buffer));

    // Close the FIFO
    close(fd);

    printf("Message received: %s\n", buffer);
    return 0;
}
