// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 25 of Hands On list 2
// 25. Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
// a. access permission
// b. uid, gid
// c. time of last message sent and received
// d. time of last change in the message queue
// d. size of the queue
// f. number of messages in the queue
// g. maximum number of bytes allowed
// h. pid of the msgsnd and msgrcv
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <time.h>

void print_time(const char *label, time_t timestamp) {
    struct tm *tm_info;
    char buffer[26];
    
    tm_info = localtime(&timestamp);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s: %s\n", label, buffer);
}

int main() {
    key_t key;
    int msgid;
    struct msqid_ds queue_info;

    // Generate a unique key (you can use your own file and project id)
    key = ftok("creatingfile.sh", 77);
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

    // Get the queue information
    if (msgctl(msgid, IPC_STAT, &queue_info) == -1) {//IPC_STAT tells msgctl to retrieve information about the message queue.
        perror("msgctl failed");
        return 1;
    }

    // Print the queue information
    printf("Access Permissions: %o\n", queue_info.msg_perm.mode);
    printf("UID: %d\n", queue_info.msg_perm.uid);
    printf("GID: %d\n", queue_info.msg_perm.gid);
    
    print_time("Last Message Sent", queue_info.msg_stime);
    print_time("Last Message Received", queue_info.msg_rtime);
    print_time("Last Change", queue_info.msg_ctime);
    
    printf("Size of Queue: %lu bytes\n", queue_info.msg_qbytes);
    printf("Number of Messages in Queue: %ld\n", queue_info.msg_qnum);
    printf("Maximum Number of Bytes Allowed: %ld\n", queue_info.msg_qbytes);
    printf("PID of Last Message Sent: %d\n", queue_info.msg_lspid);
    printf("PID of Last Message Received: %d\n", queue_info.msg_lrpid);
printf("Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);
    return 0;
}
// struct msqid_ds {
//     struct ipc_perm msg_perm; // IPC permissions
//     time_t          msg_stime; // Time of last msgsnd
//     time_t          msg_rtime; // Time of last msgrcv
//     time_t          msg_ctime; // Time of last change
//     unsigned long   msg_cbytes; // Number of bytes currently in the queue
//     msgqnum_t       msg_qnum;  // Number of messages in the queue
//     msglen_t        msg_qbytes; // Maximum number of bytes allowed in the queue
//     pid_t           msg_lspid; // PID of the last msgsnd
//     pid_t           msg_lrpid; // PID of the last msgrcv
// };
