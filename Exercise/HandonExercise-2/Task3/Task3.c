#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#define SIZE 1024

/*
----------------------------------Description--------------------------------------------------------
The code is a simple command execution program. It reads a command from the user and executes it as a
child process using the exec system call. It also supports a simple exit mechanism. When the user types
"exit", the program will terminate.
Author: Junya YANG
Created: 2024/03/14
----------------------------------End of Description-------------------------------------------------
*/

int main(void) {
    char prompt[] = "Type a command: ";
    char buf[SIZE];

    // Print a line for visual separation of each command execution
    printf("=========================================================\n");

    while (1) {
        // Display the prompt
        write(1, prompt, strlen(prompt));
        // Read the command from user input
        read(0, buf, SIZE);

        // Replace the newline character typed by the user with '\0' to terminate the string
        for (int i = 0; i < SIZE; i++) {
            if (buf[i] == '\n' || buf[i] == '\r') {
                buf[i] = '\0';
                break;
            }
        }

        // Check if the user wants to exit the program
        if (strcmp(buf, "exit") == 0) {
            break; // Exit the while loop, thus ending the program
        }

        pid_t pid;
        int status;

        // Fork a new process
        printf("Parent: Calling fork\n");
        pid = fork();
        if (pid < 0) {
            // Fork failed
            printf("Parent: Fork failed\n");
            return 1;
        } else if (pid == 0) {
            // This is the child process, executing the command
            printf("Child: now running the same program as parent, doing exec\n");
            execlp(buf, buf, NULL);
            // If execlp fails, print error message and exit child process
            printf("Child: exec failed, die\n");
            return 1;
        } else {
            // Parent process waits for the child to complete
            printf("Parent: now sleeping and waiting for child %d to end\n", pid);
            wait(&status);
            // Report that the child process has ended
            printf("Parent: finished waiting for child, child is dead\n");
            printf("Parent: Child finished, status = %d\n", status);
        }    
    }

    // Print a line for visual separation when exiting
    printf("=========================================================\n");

    return 0;
}
