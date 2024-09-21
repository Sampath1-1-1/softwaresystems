#!/bin/bash
for i in {1..30}
do
touch "Q$i.c"
echo "// Rollno:MT2024132">>Q$i.c
echo "// Name:Sampathkumar T">>Q$i.c
echo "// Question No $i of Hands On list 2">>Q$i.c
echo "#include<fcntl.h>">>Q$i.c
echo "#include<unistd.h>">>Q$i.c
echo "#include<studio.h>">>Q$i.c
echo "int main() { ">>Q$i.c
echo "return 0;">>Q$i.c
echo  "}">>Q$i.c
done
