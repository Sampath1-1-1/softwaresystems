// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 18 of Hands On list 2
// 18. Write a program to find out total number of directories on the pwd. execute ls -l | grep ^d | wc ? Use only dup2.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include<fcntl.h>

int main(){
	int fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);

	if(fork()==0){ //First child process - runs "ls -Rl"
		dup2(fd1[1], 1); //Redirect stdout to fd1[1]
		close(fd1[0]); // Close unused read end of fd1
		close(fd2[0]); //Close the write end of fd1 after dup
		close(fd2[1]);	//Close both ends of fd2
		close(fd2[0]);
		execlp("ls", "ls", "-Rl", (char *)NULL);
	}
	else{
		if(fork()==0){ //Second child process - runs "grep ^d" //filters directories
			dup2(fd1[0], 0);  //redirect stdin to fd1[0]
			dup2(fd2[1],1); //redirect stdout to fd2[1]
			close(fd1[1]); //close fd1 after redirection
			close(fd2[0]);	//close both ends of fd2 and fd1
			close(fd2[1]);
			close(fd1[0]);
			execlp("grep", "grep", "^d", (char *)NULL);
		}
		else{ //Parent runs "wc -l" //counts the number of directories
			dup2(fd2[0], 0); //redirect stdin to fd2[0]
			close(fd2[1]); //close the remaining descriptors
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[1]);
			execlp("wc", "wc", "-l", (char *)NULL);
		}
	}
	return 0;
}