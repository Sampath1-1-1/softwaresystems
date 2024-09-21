// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 30 of Hands On list1
//30.Write a program to run a script at a specific time using a Daemon process.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>



void createDaemonProcess() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        // child not created
        exit(1);
    }

    if (pid > 0) {
        // terminate parent
//         If pid > 0, it means the current process is the parent process.
// The parent process calls exit(0) to terminate itself.
// This leaves the child process running in the background.
        exit(0);
    }

    // Set the child process as the session leader
    if (setsid() < 0) {
        // exit if session is not created on child
        //        setsid() creates a new session and process group.
    //     If setsid() fails (returns -1), the child process calls exit(1) to terminate, as it was unable to create a new session.
    // Purpose: This ensures the child process becomes independent of its parent and any terminal, which is essential for a daemon process.
 exit(1);
    }

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    //newly created files and directories will have full permissions (as allowed by the system’s default settings).
    //666 for files and 777 for directories) 
    chdir("/");//it changes  the current directory to the root directory (/). 

    // Close standard input, output, and error file descriptors
    close(STDIN_FILENO);  
    //After closing, the program will no longer be able to read from the standard input. 
    close(STDOUT_FILENO); 
    close(STDERR_FILENO); 
}



int main() {
   
    // run for infinite for checking if time is there 
     int target_hour;
    int target_minute;
    
    // Prompt user for the target time
    printf("Enter Hour in 24-hour format: \n");
    scanf("%d", &target_hour);
    printf("Enter Minute: \n");
    scanf("%d", &target_minute);
     createDaemonProcess();
    while (1) {
        // getting time in UTC
        time_t now = time(NULL);
        //Passing NULL to time() for  retrieving the current time, and it is stored in the variable now.
        // converting time to local machine
        struct tm *time_info = localtime(&now);
        if (time_info->tm_hour == target_hour && time_info->tm_min == target_minute) {
                // when time is same then just trigger the execl
                    system("/home/sampathkumar/Desktop/HANDSONLIST/test.sh"); 
                    return 0;
        }
        sleep(10); 
   }
    return 0;
}
// some of the fields in struct tm include:

//     tm_sec - seconds after the minute (0-60)
//     tm_min - minutes after the hour (0-59)
//     tm_hour - hours since midnight (0-23)
//     tm_mday - day of the month (1-31)
//     tm_mon - months since January (0-11)
//     tm_year - years since 1900
//     tm_wday - days since Sunday (0-6)
//     tm_yday - days since January 1 (0-365)
//     tm_isdst - Daylight Saving Time flag
//Daemon process run in the background, allowing them to perform tasks or provide services without requiring user interaction.
// Daemons often run continuously or at scheduled intervals
// many types of Daemon createDaemonProcess
// system deamons-init/systemd: System initialization daemons that handle system startup and service management.
// udevd: Handles device events and manages device nodes in /dev.