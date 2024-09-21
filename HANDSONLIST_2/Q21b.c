// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 21 of Hands On list 2
// 21. Write two programs so that both can communicate by FIFO -Use two way communications.
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char received_message[100];
    char message_to_send[] = "Hello from Program 2";

    // Create FIFOs (if they don't exist)
    // mkfifo("myfifo1", 0666);
    // mkfifo("myfifo2", 0666);

    // Read message from Program 1
    int fd1 = open("myfifo1", O_RDONLY);
    read(fd1, received_message, sizeof(received_message));
    close(fd1);
    printf("Program 2 received: %s\n", received_message);

    // Send response to Program 1
    int fd2 = open("myfifo2", O_WRONLY);
    write(fd2, message_to_send, sizeof(message_to_send));
    close(fd2);
    printf("Program 2 sent: %s\n", message_to_send);

    return 0;
}
