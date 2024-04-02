/****************************************************************************
 *                                                                          *
 * Author		:  	Junya YANG                                              *
 *                                                                          *
 * Studdent ID	: s230026188												*
 * Console mode (command line) program.                      				*	
 *                                                                          *
 * Date			:   4.2                                          			    *
 *                                                         					*
 * Purpose		:                                                           *
 *                                                         					*
 ****************************************************************************/
 
#include <stdio.h>
#include "Queue.h"

void runTask1();
void runTask2();

int main(){
	while(1){
		printf("\nEnter a task number (1 or 2) or 0 to exit: ");
		int taskNo = 0;
		scanf("%d", &taskNo);
		switch(taskNo){
				case 0: exit(0);
				case 1: runTask1(); break;
				case 2: runTask2(); break;
				default: printf("Please enter 0, 1 or 2.\n");
		}
	}
}

void runTask1(){
	Node *Q1=NULL;
	Node *Q2=NULL;
	Node *Q3=NULL;
	Node *RecordHead=NULL;
	printf("Enter the number of processes to schedule: ");
	int num = 0;
	scanf("%d", &num);
	int count=1;
	while (count<=num){
		// 拼接两个字符 
		char name[20] = {'P', '\0'};
		name[1] = '0'+count;
		name[2] = '\0';
		int burstTime=0;
		printf("Enter the burst time of P%d: ", count);
		scanf("%d", &burstTime);
		Q1=insertAProcess(Q1, name, burstTime);
		count++;
    }
	FCFS_RR(Q1, 4, &Q2, &RecordHead);
	FCFS_RR(Q2, 8, &Q3, &RecordHead);
	FCFS(Q3, &RecordHead);
	printLinkedList(RecordHead);
}

void runTask2(){
	Node *Q1 = NULL;
	Node *Q2 = NULL;
	Node *Q3 = NULL;
	Node *Q4 = NULL;
	Node *Q5 = NULL;
	Node *RecordHead = NULL;
	printf("Enter the q uantum for Q1, Q2, Q3, and Q4:");
	int Qt1=0, Qt2=0, Qt3=0, Qt4=0;
	scanf("%d %d %d %d" , &Qt1, &Qt2, &Qt3, &Qt4);
	printf("Enter the number of processes to schedule: ");
	int num = 0;
	scanf("%d", &num);
	int count=1;
	while (count<=num){
		char name[20] = {'P', '\0'};
		name[1] = '0'+count;
		name[2] = '\0';
		int burstTime=0, priority=0;
		printf("Enter the burst time and priority of P%d: ", count);
		scanf("%d, %d", &burstTime, &priority);
		if(priority==1)
			Q1=insertAPriorityProcess(Q1, name, burstTime, priority);
		else if(priority==2)
			Q2=insertAPriorityProcess(Q2, name, burstTime, priority);
		else if(priority==3)
			Q3=insertAPriorityProcess(Q3, name, burstTime, priority);
		else if(priority==4)
			Q4=insertAPriorityProcess(Q4, name, burstTime, priority);
		else if(priority==5)
			Q5=insertAPriorityProcess(Q5, name, burstTime, priority);
		else
			printf("Invalid priority\n");
		count++;
	}
	MFQS(&Q1, &Q2, &Q3, &Q4, &Q5, &RecordHead,Qt1,Qt2,Qt3,Qt4);
	printLinkedList(RecordHead);
}
