#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct process{
	char name[20];
	int burstTime;
    int priority;
	struct process *next; 
} Node; 

Node* insertAProcess(Node *pHead, char name[], int burstTime);
Node *insertAPriorityProcess(Node *pHead, char name[], int burstTime, int priority);
Node* deleteAProcess(Node *pHead, char name[]);
void printLinkedList(Node* pHead);
void FCFS_RR(Node *pHead, int quantum, Node **nextQueue, Node **RecordHead);
void FCFS(Node *pHead, Node **RecordHead);

void MFQS(Node **Q1, Node **Q2, Node **Q3, Node **Q4, Node **Q5, Node **RecordHead,int Qt1,int Qt2,int Qt3,int Qt4);