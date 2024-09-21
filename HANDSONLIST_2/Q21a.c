// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 21 of Hands On list 2
// 21. Write two programs so that both can communicate by FIFO -Use two way communications.
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char message_to_send[] = "Hello from Program 1";
    char received_message[100];

    // Create FIFOs (if they don't exist)
    mkfifo("myfifo1", 0666);
    mkfifo("myfifo2", 0666);

    // Send message to Program 2
    int fd1 = open("myfifo1", O_WRONLY);
    write(fd1, message_to_send, sizeof(message_to_send));
    close(fd1);
    printf("Program 1 sent: %s\n", message_to_send);

    // Read response from Program 2
    int fd2 = open("myfifo2", O_RDONLY);
    read(fd2, received_message, sizeof(received_message));
    close(fd2);
    printf("Program 1 received: %s\n", received_message);

    return 0;
}


