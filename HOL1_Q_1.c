// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 1 of Hands On list1
//1. Create the following types of a files using (i) shell command (ii) system call
// a. soft link (symlink system call)
// b. hard link (link system call)
// c. FIFO (mkfifo Library Function or mknod system call)

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> // For WIFEXITED and WEXITSTATUS macros

int main() {
    // Shell commands
    const char *command1 = "ln -s switch.sh switch_symlink";
    const char *command2 = "ln switch.sh switch_hardlink";
    const char *command3 = "mkfifo switch_fifo";

    // Execute shell commands
    int ret1 = system(command1);
    int ret2 = system(command2);
    int ret3 = system(command3);

    // Check if the commands were successful
    if (ret1 == -1 || ret2 == -1 || ret3 == -1) {
        perror("system call failed");
        return 1;
    }

    // Check the exit status of the shell commands
    if (WIFEXITED(ret1) && WEXITSTATUS(ret1) == 0) {
        printf("Symbolic link created successfully.\n");
    } else {
        printf("Failed to create symbolic link.\n");
        return 1;
    }

    if (WIFEXITED(ret2) && WEXITSTATUS(ret2) == 0) {
        printf("Hard link created successfully.\n");
    } else {
        printf("Failed to create hard link.\n");
        return 1;
    }

    if (WIFEXITED(ret3) && WEXITSTATUS(ret3) == 0) {
        printf("FIFO created successfully.\n");
    } else {
        printf("Failed to create FIFO.\n");
        return 1;
    }

    return 0;
}
