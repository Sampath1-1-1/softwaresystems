// Rollno:MT2024132
//Name:Sampathkumar T
// Question No 1 of Hands On list1
//1. Create the following types of a files using (i) shell command (ii) system call
// a. soft link (symlink system call)
// b. hard link (link system call)
// c. FIFO (mkfifo Library Function or mknod system call)

 #include<stdio.h>
#include<sys/stat.h> //used for defining the structure and constants needed to obtain information about files.
#include<stdlib.h> //standard library of c.decalres various utility functions for type conversions,memory allocations,algorithms
#include<fcntl.h>   //used for file related operations
#include<unistd.h> //it provides access for various system calls and functions
//file descriptor is unique  integer assinged by OS when the file is opened
int main() { 
       //declaring the names of files
       char *myOwnFile="MyOwnFile";
       char *symLinkFile="symlink";
       char *hardLinkFile="hardLinkFile";
       char *fifoFile="Filofile";         //fileofFile is a character pointer which points to first character of the string Filofile
       FILE *fp=fopen(myOwnFile,"w");     //fp is file pointer .fp will be used to refer to the file that is being opened.
       //fopen is a function takes two  parameter .1.name of file to be opened .2 specifies the mode 
       //fopen returns null when file is not opened successfully
       if(fp==NULL){
              perror("error is occured when creating original file");
              return 0;
              // If fopen succeeds, file will be a valid pointer to a FILE object
                    }
                                 else
    printf("File opened successfully.\n");
              //close the file pointer;
              fclose(fp);
              //int symlink(const char *target ,const char *linkpath);
              //it will create new file like shortcut that points to target file
           //if original file is deleted,moved,removed the symbolic link may become "broken"
           //three errors
           //1.EEXIST-sym link already exist;
           //2.ENDENT-Target file not exist;
           //3.EACCES-permision denied
           if(symlink(myOwnFile,symLinkFile)==-1){//symlink function return -1 when symlink cannot be created
              perror("error occurrred when creating symbolic link");
              return 1;
           }
           printf("symbolic link created: %s -> %s\n",symLinkFile,myOwnFile);

           //hardlink is just a additional data entry that points to the same original file Inode
           //both are same and sharing the same data block changes made to one file will reflect another file
           if(link(myOwnFile,hardLinkFile)==-1){
              perror("error occurrred when creating hardlink file");
              return 1;
           }
           printf("hard link created: %s -> %s\n",hardLinkFile,myOwnFile);
           //4.create a FIFO names pipe.it is type of IPC mechanism
           if(mkfifo(fifoFile,666)==-1){//mkfifo function is used to create fifo pipe
           //we giving read ,write permissons to all 
              perror("error creating FIFO");
              return 1;
           }
           printf("FIFO  created: %s\n",fifoFile);
           return 0;

}
