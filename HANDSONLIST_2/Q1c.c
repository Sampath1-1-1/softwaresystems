// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 1 of Hands On list 2
// 1. Write a separate program (for each time domain) to set a interval timer in 10sec and 10micro second
// a. ITIMER_REAL
// b. ITIMER_VIRTUAL
// c. ITIMER_PROF
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGPROF
void handle_prof_alarm(int signum) {
    printf("Timer expired (ITIMER_PROF)\n");
}

int main() {
    struct itimerval timer;

    // Set signal handler for SIGPROF
    signal(SIGPROF, handle_prof_alarm);

    // Set timer to 10 seconds and 10 microseconds
    timer.it_value.tv_sec = 10;        // First expiration in 10 seconds
    timer.it_value.tv_usec = 10;       // and 10 microseconds
    timer.it_interval.tv_sec = 10;     // Interval for periodic timer (10 seconds)
    timer.it_interval.tv_usec = 10;    // and 10 microseconds

    // Start the timer (ITIMER_PROF)
    setitimer(ITIMER_PROF, &timer, NULL);

    // Do some busy work
    while (1) {
        // This timer decrements when the process or the system is running
    }

    return 0;
}
