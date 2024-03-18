#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>

int forknum = 0;

int main(){
    // while(1){
        printf("Forking %d\n", forknum++);
        fork();
        break;
    // }
    return 0;
}