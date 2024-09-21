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

// Signal handler for SIGALRM
void handle_alarm(int signum) {
    printf("Timer expired (ITIMER_REAL)\n");
}

int main() {
    struct itimerval timer;
    //Stores the timing values for the interval timer.

    // Set signal handler for SIGALRM
    signal(SIGALRM, handle_alarm);
//  SIGALRM specifies the signal to catch.. handle_alarm specifies the function to call (our handle_alarm() function).

    // Set timer to 10 seconds and 10 microseconds
    timer.it_value.tv_sec = 10;        // First expiration in 10 seconds
    timer.it_value.tv_usec = 10;       // and 10 microseconds
    timer.it_interval.tv_sec = 10;     // Interval for periodic timer (10 seconds)
    timer.it_interval.tv_usec = 10;    // and 10 microseconds

    // Start the timer (ITIMER_REAL)
    setitimer(ITIMER_REAL, &timer, NULL);//The setitimer() function starts the timer.
  //Specifies the type of timer (real-time clock). When the timer expires, it sends a SIGALRM signal to the process.  
  //&timer: Points to the itimerval structure that holds the timer settings (i.e., when and how often the timer will expire)
    // Wait indefinitely
    while (1) {
        pause();  // Wait for signal
    }

    return 0;
}
// a. ITIMER_REAL
// Decreases in real-time, which includes the time when the process is running and when it is not (i.e., the system clock).
// b. ITIMER_VIRTUAL
//  Decreases only when the process is running (user CPU time). It does not count time when the process is blocked or waiting.
//  c. ITIMER_PROF

//  Timer Type	      Counts Time When...	                         Signal Sent	               Common Use Case
// ITIMER_REAL	      Process is running or blocked	                   SIGALRM	                  Real-time-based events like timeouts
// ITIMER_VIRTUAL	  Process is executing (user mode)	              SIGVTALRM	                   Measure CPU time used by the process (user space)
// ITIMER_PROF	      Process is executing (user + system mode)	       SIGPROF	                  Measure CPU time spent in both user and system space for profiling