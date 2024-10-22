
#include <stdio.h> // Import for `printf` & `perror` functions
#include <errno.h> // Import for `errno` variable

#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `accept`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `accept` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture
#include <sys/ipc.h>  // For `IPC_CREAT` and shared memory keys
#include <sys/shm.h> 

#include <string.h>  // Import for string functions
#include <stdbool.h> // Import for `bool` data type
#include <stdlib.h>  // Import for `atoi` function

#include "./admin/admin.h"
#include "./customer/customer.h"
#include "./record/account.h"
#include "./resources/constantterm.h"
#include "./employee/employee.h"
#include "./manager/manager.h"
#include "./record/employee.h"
#include "./record/loanapply.h"
#include "./record/feedback.h"

void connection_handler(int connFD); // Handles the communication with the client
 int shmid;
    int *tot_mem;
void shared_mem(){
    
    key_t key= ftok("miniproject",1234);
    // Create shared memory segment
    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error creating shared memory");
        exit(1);
    }

    // Attach the shared memory to our process
    tot_mem = (int *)shmat(shmid, NULL, 0);
    if (tot_mem == (int *)-1) {
        perror("Error attaching shared memory");
        exit(1);
    }

    // Initialize the shared memory value
    *tot_mem = 0;  // Set the value to 0 initially

    printf("Total client: %d\n", (*tot_mem));

    // Simulate some operation: Increment the value
 

}
void detach_sharedmemory(){

    // Detach the shared memory from the process
    if (shmdt(tot_mem) == -1) {
        perror("Error detaching shared memory");
        exit(1);
    }
}
void main()
{
    int socketFileDescriptor, socketBindStatus, socketListenStatus, connectionFileDescriptor;
    struct sockaddr_in serverAddress, clientAddress;
    shared_mem();

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating server socket!");
        _exit(0);
    }

    serverAddress.sin_family = AF_INET;                // IPv4
    serverAddress.sin_port = htons(8081);              // Server will listen to port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    socketBindStatus = bind(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (socketBindStatus == -1)
    {
        perror("Error while binding to server socket!");
        _exit(0);
    }

    socketListenStatus = listen(socketFileDescriptor, 10);
    if (socketListenStatus == -1)
    {
        perror("Error while listening for connections on the server socket!");
        close(socketFileDescriptor);
        _exit(0);
    }

    int clientSize;
    while (1)
    {
        clientSize = (int)sizeof(clientAddress);
        connectionFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *)&clientAddress, &clientSize);
        if (connectionFileDescriptor == -1)
        {
            perror("Error while connecting to client!");
            close(socketFileDescriptor);
        }
        else
        {
            if (!fork())
            {
                    (*tot_mem)++;
                // Child will enter this branch
                printf("Total client: %d\n", (*tot_mem));
            

                connection_handler(connectionFileDescriptor);
                close(connectionFileDescriptor);
                _exit(0);
            }
        }
    }

    detach_sharedmemory();
    close(socketFileDescriptor);
}

void connection_handler(int connectionFileDescriptor)
{
    printf("Client has connected to the server!\n");

    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;
    int userChoice;

    writeBytes = write(connectionFileDescriptor, INITIAL_PROMPT, strlen(INITIAL_PROMPT));
    if (writeBytes == -1)
        perror("Error while sending first prompt to the user!");
    else
    {
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
            perror("Error while reading from client");
        else if (readBytes == 0)
            printf("No data was sent by the client");
        else
        {
            userChoice = atoi(readBuffer);

            switch (userChoice)
            {
            case 1:
                // Admin

                admin_operation_handler(connectionFileDescriptor);
                break;
            case 2:
                // Customer
                customer_operation_handler(connectionFileDescriptor);
                break;
            case 3:
            // employee
                employee_operation_handler(connectionFileDescriptor);
                break;
            case 4:
                manager_operation_handler(connectionFileDescriptor);
                break;
            default:
                // Exit
                 // Invalid choice, notify the client and terminate the connection
                strcpy(writeBuffer, "Invalid choice! Terminating connection.\n$");
                writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                if (writeBytes == -1)
                    perror("Error while sending termination message to client");
                else
                    printf("Sent termination message to client.\n");
                break;
            }
        }
    }

    printf("Terminating connection to client!\n");
    (*tot_mem)--;
    
   char message[100];
    sprintf(message, "Client disconnected. Total clients: %d\n",*tot_mem);
    write(STDOUT_FILENO, message, strlen(message));

    close(connectionFileDescriptor); // Close the connection
    
}
