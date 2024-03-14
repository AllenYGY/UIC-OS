#include <stdio.h>
#include <string.h>
#define SIZE 1024

int main(void)
{

    char prompt[] = "Type a command: ";
    char buf[SIZE]; // Ask the user to type a command:

    printf(prompt); 
    /* 
     Read from the standard input the command typed by the user (note 
     that fgets also puts into the array buf the ’\n’ character typed 
     by the user when the user presses the Enter key on the keyboard): 
    */
    fgets(buf, SIZE, stdin); // Replace the Enter key typed by the user with ’\0’:

    for (int i = 0; i < SIZE; i++)
    {
        if (buf[i] == '\n' || buf[i] == '\r')
        {
            buf[i] = '\0';
            break;
        }
    }
    // Execute the command typed by the user (only prints it for now):
    printf("Executing command: %s\n", buf);
    return 0;
}