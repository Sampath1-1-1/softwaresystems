// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 13 of Hands On list1
//13.Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
//verify whether the data is available within 10 seconds or not (check in $man 2 select).
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>

int main() {
    fd_set readfds;
    //The fd_set type is a data structure used by the select system call to specify which file descriptors you want to monitor
    //It represents a collection of file descriptors that you want to monitor. 
    struct timeval tout;
    //The struct timeval type is used to specify the timeout interval for the select function
    int result;

    // Initialize the file descriptor set
    FD_ZERO(&readfds);
//     FD_ZERO(fd_set *set): Clears the set, so it becomes an empty set.
// FD_SET(int fd, fd_set *set): Adds a file descriptor to the set.
// FD_CLR(int fd, fd_set *set): Removes a file descriptor from the set.
// FD_ISSET(int fd, fd_set *set): Checks if a file descriptor is part of the set.
//     FD_SET(STDIN_FILENO, &read_fds); // Add stdin to the set

    // Set the timeout value
    tout.tv_sec = 10;  // 10 seconds
    tout.tv_usec = 0;  // 0 microseconds

    // Wait for input or timeout
    result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tout);
    //The select function is used to monitor multiple file descriptors to see if they are ready for some class of I/O operations
    //int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
//     nfds: The highest-numbered file descriptor in any of the three sets, plus one.
// readfds: A pointer to a set of file descriptors to be checked for readability.
// writefds: A pointer to a set of file descriptors to be checked for writability.
// exceptfds: A pointer to a set of file descriptors to be checked for exceptional conditions.
// timeout: A pointer to a struct timeval specifying the maximum time to wait for an event.

    if (result == -1) {
        perror("select function is making error");
        return EXIT_FAILURE;
    } else if (result == 0) {
        //result == 0: The timeout period expired, and no file descriptors were ready for the specified operations.
        printf("No input received within 10 seconds.\n");
    } else {
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            printf("Input is available now.\n");
        }
    }

    return EXIT_SUCCESS;
}
