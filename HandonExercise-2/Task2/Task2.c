#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>

int main(void){
    pid_t pid;
    int status;
    printf("Parent: Calling fork\n");
    pid = fork();
    if (pid < 0){
        printf("Parent: Fork failed\n");
        return 1;
    }
    else if (pid==0)
    {
        printf("Child: now running the same program as parent, doing exec\n");
        execlp("buf","buf",NULL);
        printf("Child: exec faild, die\n");
        return 1;
    }
    else
    {
        printf("Parent: now sleeping and waiting for child %d to end\n", pid);
        wait(&status);
        printf("Parent: finished waiting for child, child is dead\n");
        printf("Parent: Child finished, status = %d\n", status);
        return 0;
    }    
}