// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 28 of Hands On list 2
// 28. Write a program to change the existing message queue permission. (use msqid_ds structure)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds change;

    // Generate unique key for message queue
    key = ftok("creatingfile.sh", 25); // Path and id to generate key

    // Create or get message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // Get current message queue info
    if (msgctl(msgid, IPC_STAT, &change) == -1) {
        //used to perform various control operations on System V message queues. 
        // It allows you to retrieve information about the message queue, modify its parameters, and remove the queue when it's no longer needed.
        perror("msgctl get failed");
        return 1;
        //IPC_STAT tells msgctl to retrieve the current status of the message queue and store it in the structure pointed to by the third argument (&change).
    }

    // Change the permissions (mode) to allow read and write for owner and group
    change.msg_perm.mode = 0660; // rw-rw---- (owner and group can read/write)

    // Set the updated info back to the message queue
    if (msgctl(msgid, IPC_SET, &change) == -1) {
        perror("msgctl set failed");
        return 1;
    }

    printf("Message queue permissions changed successfully.\n");

    return 0;
}
// msg_perm: A structure of type struct ipc_perm that defines the access permissions for the message queue (user ID, group ID, and permission bits).
// msg_stime: Time of the last message sent (of type time_t).
// msg_rtime: Time of the last message received (of type time_t).
// msg_ctime: Time of the last change in the message queue's state (of type time_t).
// msg_qnum: Number of messages currently in the queue.
// msg_qbytes: Maximum number of bytes allowed in the queue.
// msg_lspid: Process ID of the last process that sent a message.
// msg_lrpid: Process ID of the last process that received a message.