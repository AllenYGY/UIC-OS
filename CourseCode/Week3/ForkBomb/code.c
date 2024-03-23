#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>

//*  Fork bomb

//* This program will keep forking new processes until the system runs out of resources

int forknum = 0;

int main(){
    while(1){
        printf("Forking %d\n", forknum++);
        fork();
    }
    return 0;
}