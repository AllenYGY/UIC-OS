#include <string.h>
#include <unistd.h>
#define SIZE 1024

/*
----------------------------------Description--------------------------------------------------------
The code is a simple program that takes a command from the user and runs it using the exec system call.
Author: Junya YANG
Created: 2024/03/14
----------------------------------End of Description-------------------------------------------------
*/

int main(void)
{

    char prompt[] = "Type a command: ";
    char buf[SIZE];
    // Ask the user to type a command:
    write(1, prompt, strlen(prompt));
    /* 
     Read from the standard input the command typed by the user (note 
     that fgets also puts into the array buf the ’\n’ character typed 
     by the user when the user presses the Enter key on the keyboard): 
    */
    read(0, buf, SIZE);
    
     // Replace the Enter key typed by the user with ’\0’:

    for (int i = 0; i < SIZE; i++)
    {
        if (buf[i] == '\n' || buf[i] == '\r')
        {
            buf[i] = '\0';
            break;
        }
    }
    char Bye[]="Executing command:";
    write(1,Bye,strlen(Bye));    
    // Execute the command typed by the user (only prints it for now):
    // output the user typed command:
    write(1, buf, strlen(buf)); 
    write(1, "\n", 1);
    return 0;
}