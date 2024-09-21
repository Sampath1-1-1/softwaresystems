// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 34 of Hands On list 2
// 34. Write a program to create a concurrent server.
// a. use fork
// b. use pthread_create
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 2048
void handle_client(int client_socket) {
    char buffer[BUF_SIZE] = {0};

    // Read data from the client
    read(client_socket, buffer, BUF_SIZE);
    printf("Message from the client: %s\n", buffer);

    // Send a response back to the client
    const char *response = "Hii from fork server";
    send(client_socket, response, strlen(response), 0);
    printf("Response sent to client.\n");

    // Close the client socket
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening  the port %d\n", PORT);

    while (1) {
        // Accept a new connection
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }

        // Create a new process to handle the client
        if (fork() == 0) {
            close(server_fd); // Close the listening socket in the child process
            handle_client(client_socket);
            exit(0); // Exit child process after handling
        }
        close(client_socket); // Close the client socket in the parent process
    }

    return 0;
}

