#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

__thread int var = 5;

void* worker1(void*arg);
void* worker2(void*arg);

int main(){
    pthread_t pid1, pid2;
    pthread_create(&pid1, NULL, worker1, NULL);
    pthread_create(&pid2, NULL, worker2, NULL);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    return 0;
}

void* worker1(void*arg){
    var++;
    printf("Worker1: var = %d\n", var);
    return NULL;
}

void* worker2(void*arg){
    sleep(1);
    var+=2;
    printf("Worker2: var = %d\n", var);
    return NULL;
}