// Rollno:MT2024132
// Name:Sampathkumar T
// Question No 22 of Hands On list 2
// 22. Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

int main(void){
	int fd, retval;
	char buff[80];
	fd_set rfds;
	struct timeval tv;

	fd = open("my_fifo3", O_RDONLY);
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	tv.tv_sec=10;
	tv.tv_usec=0;
	retval=select(fd+1, &rfds, NULL, NULL, &tv);
	if(retval){
		printf(" Data is available now...\n");
	}
	else{
		printf(" No data is available within 10 seconds...\n");
		exit(0);
	}
	read(fd, buff, sizeof(buff));
	printf(" The text from FIFO: %s\n", buff);
	return 0;
}
