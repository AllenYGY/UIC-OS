#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h> 

#define TRUE 1

void *AAfunc(void *param);
void *namefunc(void *param);
void *studentIDfunc(void *param);

sem_t sem1, sem2, sem3;

int main(int argc, char *argv[]) {
	int i, scope;
	pthread_t threadID_id, threadName_id, threadAA_id; 	/* the thread identifier */
	pthread_attr_t attr; 	/* set of attributes for the thread */
	
	/* get the default attributes */
	pthread_attr_init(&attr);
	sem_init(&sem1, 0, 1);
	sem_init(&sem2, 0, 0);
	sem_init(&sem3, 0, 0);
	/* create 3 threads */
	pthread_create(&threadID_id, &attr, studentIDfunc, &threadID_id);
	pthread_create(&threadName_id, &attr, namefunc, &threadName_id);
	pthread_create(&threadAA_id, &attr, AAfunc, &threadAA_id);
	/* Now join on each thread */
	pthread_join(threadID_id, NULL);
	pthread_join(threadName_id, NULL);
	pthread_join(threadAA_id, NULL);
    printf("Program is done\n");
	return 0;
}
void *studentIDfunc(void *param) {
	sem_wait(&sem1); // Acquire the semaphore
	for (int i = 0; i < 10; i++)
	   printf("%d: My student ID is 2230026188\n", i);
	sem_post(&sem1); // Release the semaphore 1
	sem_post(&sem2); // Release the semaphore 2
	pthread_exit(0);

}
void *namefunc(void *param) {
	sem_wait(&sem2); // Acquire the semaphore 2
	printf("My name is Junya YANG\n");
	sem_post(&sem2); // Release the semaphore 2
	sem_post(&sem3); // Release the semaphore 3
	pthread_exit(0);
}
void *AAfunc(void *param) {
	sem_wait(&sem3); // Acquire the semaphore 3
	printf("My AA is Nina Yingran MA\n");
	sem_post(&sem3); // Release the semaphore 3
	pthread_exit(0);
}


/***
 * Task1 output of first time:
0: My student ID is 222222222222
1: My student ID is 222222222222
2: My student ID is 222222222222
3: My student ID is 222222222222
4: My student ID is 222222222222
5: My student ID is 222222222222
6: My student ID is 222222222222
7: My student ID is 222222222222
8: My student ID is 222222222222
My name is Judy
9: My student ID is 222222222222
Program is done
My AA is HOLY ONE
*/

/***
 * Task1 output of second time:
0: My student ID is 222222222222
1: My student ID is 222222222222
2: My student ID is 222222222222
3: My student ID is 222222222222
4: My student ID is 222222222222
5: My student ID is 222222222222
6: My student ID is 222222222222
7: My student ID is 222222222222
8: My student ID is 222222222222
9: My student ID is 222222222222
My name is Judy
My AA is HOLY ONE
Program is done
*/

/***
 * Task1 output of third time:
0: My student ID is 222222222222
1: My student ID is 222222222222
2: My student ID is 222222222222
3: My student ID is 222222222222
4: My student ID is 222222222222
5: My student ID is 222222222222
6: My student ID is 222222222222
7: My student ID is 222222222222
8: My student ID is 222222222222
Program is done
My AA is HOLY ONE
9: My student ID is 222222222222
My name is Judy
*/

/***
 * Task1 Explain why:
For each thread, there is a timeslice (quantum), and when the time slice is finished.
It will be run by another thread, so the output order will be inconsistent
*/

/***
 * Output of Task2:
0: My student ID is 2230026188
1: My student ID is 2230026188
2: My student ID is 2230026188
3: My student ID is 2230026188
4: My student ID is 2230026188
5: My student ID is 2230026188
6: My student ID is 2230026188
7: My student ID is 2230026188
8: My student ID is 2230026188
9: My student ID is 2230026188
My name is Junya YANG
My AA is Nina Yingran MA
Program is done
*/