// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 27 of Hands On list 2
// 27. Write a program to receive messages from the message queue.
// a. with 0 as a flag
// b. with IPC_NOWAIT as a flag

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Define the structure of the message
#define MSG_SIZE 100
struct message {
    long msg_type; // Message type (must be > 0)
    char msg_text[MSG_SIZE];
};

void receive_message(int msgid, int flag) {
    struct message msg;
    ssize_t msg_size;
    
    msg_size = msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, flag);
    if (msg_size == -1) {
        perror("msgrcv failed");
        return;
    }
    
    printf("Received Message: %s\n", msg.msg_text);
}

int main() {
    key_t key;
    int msgid;

    // Generate a unique key
    key = ftok("creatingfile.sh", 65);
    if (key == -1) {
        perror("ftok failed");
        return 1;
    }

    // Get the message queue ID
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // Receive message with 0 flag (wait indefinitely)
    printf("Receiving message with 0 flag.\n");
    receive_message(msgid, 0);

    // Receive message with IPC_NOWAIT flag (non-blocking)
    printf("Receiving message with IPC_NOWAIT flag \n");
    receive_message(msgid, IPC_NOWAIT);
    //When using IPC_NOWAIT with the msgrcv function, the call will return immediately if no messages are available in the queue, 
    // rather than blocking and waiting for a message to arrive.

    return 0;
}
