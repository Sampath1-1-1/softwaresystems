// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 26 of Hands On list 2
// 26. Write a program to send messages to the message queue. Check $ipcs -q
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

int main() {
    key_t key;
    int msgid;
    struct message msg;
    //struct message is defined to include:
      // Message Type: To identify the type of message. It helps in filtering messages when receiving.
    // Message Data: The actual content or payload of the message.

    // Generate a unique key
    key = ftok("creatingfile.sh", 65);
    if (key == -1) {
        perror("ftok failed");
        return 1;
    }

    // Create or access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // Prepare the message
    msg.msg_type = 1; // Message type (1 is commonly used for messages)
    strncpy(msg.msg_text, "Hello, this is a test message", MSG_SIZE);

    // Send the message
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {//msgsnd is used to send a message to a message queue.
    //msgid is the identifier of the message queue.
    //&msg points to the message structure that you want to send.
        perror("msgsnd failed");
        return 1;
    }

    printf("Message sent successfully!\n");

     if (msgrcv(msgid, &msg, MSG_SIZE, 1, 0) == -1) {//int msgrcv(int msgid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    // Print the received message
    printf("Received message: %s\n", msg.msg_text);
    
    return 0;

    return 0;
}
// int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
// msgid is the identifier of the message queue.
// msgflg: Flags that modify the behavior of the msgsnd call. In this case, 0 means no special flags are set
