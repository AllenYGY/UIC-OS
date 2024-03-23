#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define the number of threads
#define NUM_THREADS 11
// Define the size of the sudoku
#define SIZE 9

// Use global variables to store the sudoku matrix and thread ID
int sudoku[SIZE][SIZE];
pthread_t tid[NUM_THREADS];

// Define the parameter structure
typedef struct
{
    int row;
    int column;
} parameters;

// Declare thread functions
// Check the row
void *checkRow(void *param);
// Check the column
void *checkColumn(void *param);
// Check the square
void *checkSquare(void *param);

int main(int argc, char *argv[])
{
    pthread_attr_t attr; // Set the thread attribute
    char *token;  // Define the token
    if (argc != 2)
    {
        fprintf(stderr, "You need to provide a sudoku data file!\nUsage: ./sudoku <filename>\n"); // Print the error message
        return -1;
    }
    FILE *filePointer; // Define the file pointer
    char buffer[100]; // Define the buffer
    filePointer = fopen(argv[1], "r"); // Open the file
    if (filePointer == NULL)
    {
        fprintf(stderr, "Can't Find File!\n"); // Print the error message
        return 1;
    }
    int row = 0; // Define the row
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL) // Read the file
    {
        token = strtok(buffer, ","); // Split the string
        for (int col = 0; token != NULL; col++) // Loop through the columns
        {
            sudoku[row][col] = atoi(token); // Convert the string to integer
            token = strtok(NULL, ","); // Split the string
        }
        row++;
    }
    fclose(filePointer); // Close the file
    pthread_attr_init(&attr); // Initialize the thread attribute
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // Set the thread attribute
    // Create and start the threads
    pthread_create(&tid[0], &attr, checkRow, NULL); // Create the thread to check the row
    pthread_create(&tid[1], &attr, checkColumn, NULL); // Create the thread to check the column
    parameters data[9]; // Define the data
    for (int i = 0; i < 9; ++i) {
        data[i].row = (i / 3) * 3; // Define the row
        data[i].column = (i % 3) * 3; // Define the column
        pthread_create(&tid[i + 2], &attr, checkSquare, (void *)&data[i]); // Create the thread to check the square
    }
    bool isSudoku = true; // Define the isSudoku
    void *status; // Define the status
    for (int i = 0; i < 11; i++) // Loop through the threads
    {
        pthread_join(tid[i], &status); // Join the thread
        if (*(bool *)status == false) // Check the status
        {
            isSudoku = false; // Set the isSudoku to false
        }
        free(status);
    }
    for (int i = 0; i < 9; i++) //Output the sudoku
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    if (isSudoku) // Check the sudoku
    {
        printf("This Sudoku solution is valid\n");
    }
    else
    {
        printf("Invalid Sudoku solution\n");
    }
    return 0;
}

void *checkRow(void *param)
{
    bool *isvaild=(bool *)malloc(sizeof(bool));
    *isvaild=true;
    for (int i = 0; i < 9; i++)
    {
        int check[10] = {0};
        for (int j = 0; j < 9; j++)
        {
            if (check[sudoku[i][j]] == 1)
            {
                *isvaild=false;
                pthread_exit(isvaild);
            }
            check[sudoku[i][j]] = 1;
        }
    }
    pthread_exit(isvaild);
}

void *checkColumn(void *param)
{
    bool *isvaild=(bool *)malloc(sizeof(bool));
    *isvaild=true;
    for (int i = 0; i < 9; i++)
    {
        int check[10] = {0};
        for (int j = 0; j < 9; j++)
        {
            if (check[sudoku[j][i]] == 1)
            {
                *isvaild=false;
                pthread_exit(isvaild);
            }
            check[sudoku[j][i]] = 1;
        }
    }
    pthread_exit(isvaild);
}

void *checkSquare(void *param)
{
    int row = ((parameters *)param)->row;
    int column = ((parameters *)param)->column;
    int check[10] = {0};
    bool *isvaild=(bool *)malloc(sizeof(bool));
    *isvaild=true;
    for (int i = row; i < row + 3; i++)
    {
        for (int j = column; j < column + 3; j++)
        {
            if (check[sudoku[i][j]] == 1)
            {
                *isvaild=false;
                pthread_exit(isvaild);
            }
            check[sudoku[i][j]] = 1;
        }
    }
    pthread_exit(isvaild);
}