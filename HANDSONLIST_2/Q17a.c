// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 17 of Hands On list 2
// 17. Write a program to execute ls -l | wc.
// a. use dup
// b. use dup2
// c. use fcntl
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int fd[2];
	pipe(fd);

if (fork() == 0) {		close(1); //closing std output
		close(fd[0]);
		dup(fd[1]); //since 1 is the lowest fd free it will point to fd[1]
		execlp("ls", "ls", "-l", (char *)NULL);
	}
	else{
		close(0); //std input closed
		close(fd[1]);
		dup(fd[0]);
//		wait(20);
		execlp("wc", "wc", (char *)NULL);
	}
	return 0;
}