// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 33 of Hands On list 2
// 33. Write a program to communicate between two machines using socket server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hii from the  client ";
    char buffer[BUF_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); // Convert port to network byte order

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { // Change to server's IP
        printf("Invalid address or Address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    // Send message to the server
    send(sock, message, strlen(message), 0);
    printf("Message is  sent to server.\n");

    // Read response from the server
    read(sock, buffer, BUF_SIZE);
    printf("Response from the  server: %s\n", buffer);

    // Close the socket
    close(sock);

    return 0;
}
