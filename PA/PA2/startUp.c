/****************************************************************************
 *                                                                          *
 * Author        : Junya YANG                                              *
 *                                                                          *
 * Student ID    : s230026188                                               *
 * Console mode (command line) program.                                     *
 *                                                                          *
 * Date          : 4.2                                                      *
 *                                                                          *
 * Purpose       : This program demonstrates a multi-level feedback queue   *
 *                 scheduling system for processes. The user can run two    *
 *                 different scheduling tasks to simulate how processes     *
 *                 are managed in a computing environment.                  *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include "Queue.h" // Custom header file for queue operations.

// Declaration of functions to run the scheduling tasks.
void runTask1();
void runTask2();

int main(){
    // Main loop: allows the user to select a task to run or exit the program.
	while(1){
		printf("\nEnter a task number (1 or 2) or 0 to exit: ");
		int taskNo = 0;
		scanf("%d", &taskNo);
		switch(taskNo){
				case 0: exit(0); // Exit the program.
				case 1: runTask1(); break; // Run the first scheduling task.
				case 2: runTask2(); break; // Run the second scheduling task.
				default: printf("Please enter 0, 1 or 2.\n"); // Error handling for invalid input.
		}
	}
}

// Task 1: Demonstrates scheduling with Fixed Quantum Slices across multiple queues.
void runTask1(){
	Node *Q1=NULL; // Queue 1
	Node *Q2=NULL; // Queue 2
	Node *Q3=NULL; // Queue 3
	Node *RecordHead=NULL; // Linked list to record the scheduling order.
	printf("Enter the number of processes to schedule: ");
	int num = 0;
	scanf("%d", &num);
	int count=1;
	while (count<=num){
		char name[20] = {'P', '\0'}; // Process name initialization.
		name[1] = '0'+count; // Naming process as P1, P2, etc.
		int burstTime=0; // Burst time for the process.
		printf("Enter the burst time of P%d: ", count);
		scanf("%d", &burstTime);
		// Insert the process into Queue 1.
		Q1=insertAProcess(Q1, name, burstTime);
		count++;
    }
	// Schedule processes using First-Come-First-Served and Round Robin algorithms.
	FCFS_RR(Q1, 4, &Q2, &RecordHead);
	FCFS_RR(Q2, 8, &Q3, &RecordHead);
	FCFS(Q3, &RecordHead);
	// Print the final scheduling order.
	printLinkedList(RecordHead);
}

// Task 2: Implements a Multi-level Feedback Queue Scheduling system.
void runTask2(){
	Node *Q1 = NULL; // Queue 1 with the highest priority.
	Node *Q2 = NULL; // Queue 2
	Node *Q3 = NULL; // Queue 3
	Node *Q4 = NULL; // Queue 4
	Node *Q5 = NULL; // Queue 5 with the lowest priority.
	Node *RecordHead = NULL; // Linked list to record the scheduling order.
	printf("Enter the quantum for Q1, Q2, Q3, and Q4: ");
	int Qt1=0, Qt2=0, Qt3=0, Qt4=0;
	scanf("%d %d %d %d" , &Qt1, &Qt2, &Qt3, &Qt4);
	printf("Enter the number of processes to schedule: ");
	int num = 0;
	scanf("%d", &num);
	int count=1;
	while (count<=num){
		char name[20] = {'P', '\0'}; // Process name initialization.
		name[1] = '0'+count; // Naming process as P1, P2, etc.
		int burstTime=0, priority=0; // Burst time and priority for the process.
		printf("Enter the burst time and priority of P%d: ", count);
		scanf("%d,%d", &burstTime, &priority);
		// Insert the process into the appropriate queue based on priority.
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
		count++;
	}
	// Schedule processes across multiple queues with different quantum times.
	MFQS(&Q1, &Q2, &Q3, &Q4, &Q5, &RecordHead, Qt1, Qt2, Qt3, Qt4);
	// Print the final scheduling order.
	printf("The scheduling sequence is:");
	printLinkedList(RecordHead);
}
