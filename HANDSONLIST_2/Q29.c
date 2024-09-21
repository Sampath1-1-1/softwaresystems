// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 29 of Hands On list 2
// 29. Write a program to remove the message queue.
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h> 

int main() {
    key_t key;
    int msgid;

    // Generate a unique key for the message queue
    key = ftok("creatingfile.sh", 95);  // Path and id to generate key

    // Create or get the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {//IPC_RMID command is used to remove message queues 
    //This operation ensures that the message queue and its associated resources are freed.
        perror("msgctl remove failed");
        return 1;
    }

    printf("Message queue removed successfully.\n");
if ((msgid = msgget(key, 0666)) == -1) {
        printf("Failed to access message queue after removal: %s\n", strerror(errno));
    } else {
        printf("Unexpectedly accessed message queue with ID: %d\n", msgid);
    }
    return 0;
}
