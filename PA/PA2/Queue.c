/****************************************************************************
 *                                                                          *
 * Author		:  	 Junya YANG                                             *
 *                                                                          *
 * Studdent ID	:  s230026188 												*
 * Console mode (command line) program.                                     *
 *                                                                          *
 * Date			:                                             			    *
 *                                                         					*
 * Purpose		:                                                           *
 *                                                         					*
 ****************************************************************************/

#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printLinkedList(Node *pHead)
{
	Node *pNode = pHead;
	while (pNode->next != NULL)
	{
		printf("%s, ", pNode->name);
		pNode = pNode->next;
	}
	printf("%s", pNode->name);
	puts("");
	return;
}

Node *insertAProcess(Node *pHead, char name[], int burstTime)
{
	// enter name and burstTime
	Node *pNewProcess;
	Node *pNode1 = pHead, *pNode2;
	pNewProcess = (Node *)malloc(sizeof(Node));
	strcpy(pNewProcess->name, name);
	pNewProcess->burstTime = burstTime;

	pNewProcess->next = NULL;

	if (pNode1 == NULL)
		return pNewProcess;

	// Find the position to insert the new process
	while (pNode1 != NULL)
	{
		pNode2 = pNode1;
		pNode1 = pNode1->next;
	}
	// Insert the new process
	pNode2->next = pNewProcess;
	return pHead;
}

Node *deleteAProcess(Node *pHead, char name[])
{
	Node *pNode1 = pHead, *pNode2=NULL;
	while (pNode1 != NULL && strcmp(pNode1->name, name) != 0)
	{
		pNode2 = pNode1;
		pNode1 = pNode1->next;
	}
	if (pNode1 == NULL)
		return pHead;

	if (pNode1 == pHead)	  // for the case that head's name is name
		pHead = pNode1->next; // the second node will be the head
	else					  // pNode1 -> name is name
		pNode2->next = pNode1->next;

	free(pNode1); // free the memory for original head
	return pHead;
}

void FCFS_RR(Node *pHead, int quantum, Node **nextQueue, Node **RecordHead) {
    Node *pNode = pHead;
    Node *tmp = NULL;
    while (pNode != NULL) {
        tmp = pNode->next; 
        if (pNode->burstTime > quantum) {
            pNode->burstTime -= quantum;
            *nextQueue = insertAProcess(*nextQueue, pNode->name, pNode->burstTime);
        } else {
            pNode->burstTime = 0;
        }

        *RecordHead = insertAProcess(*RecordHead, pNode->name, pNode->burstTime);
        pHead = deleteAProcess(pHead, pNode->name);
        pNode = tmp; 
    }
}

void FCFS(Node *pHead, Node **RecordHead)
{
	FCFS_RR(pHead, 10000000, NULL, RecordHead);
}

Node *insertAPriorityProcess(Node *pHead, char name[], int burstTime, int priority)
{
	// enter name and burstTime
	Node *pNewProcess;
	Node *pNode1 = pHead, *pNode2;
	pNewProcess = (Node *)malloc(sizeof(Node));
	strcpy(pNewProcess->name, name);
	pNewProcess->burstTime = burstTime;
	pNewProcess->priority = priority;

	pNewProcess->next = NULL;
	if (pNode1 == NULL)
		return pNewProcess;
	// Find the position to insert the new process
	while (pNode1 != NULL)
	{
		pNode2 = pNode1;
		pNode1 = pNode1->next;
	}
	// Insert the new process
	pNode2->next = pNewProcess;
	return pHead;
}

void MFQS(Node **Q1, Node **Q2, Node **Q3, Node **Q4, Node **Q5, Node **RecordHead,int Qt1,int Qt2,int Qt3,int Qt4){
	Node *pNode = NULL;
	FCFS_RR(*Q1, Qt1, Q2, RecordHead);
	FCFS_RR(*Q2, Qt2, Q3, RecordHead);
	FCFS_RR(*Q3, Qt3, Q4, RecordHead);
	FCFS_RR(*Q4, Qt4, Q5, RecordHead);
	FCFS(*Q5, RecordHead);
}