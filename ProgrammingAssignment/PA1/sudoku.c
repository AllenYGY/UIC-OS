#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int row;
    int column;
} parameters;

int isValid = 0;
void *checkRow(void *param);
void *checkColumn(void *param);
void *checkSquare(void *param);

int sudoku[9][9];

int main(int argc, char *argv[])
{
    pthread_t tid[11];
    pthread_attr_t attr; // 线程属性

    char *token;
    if (argc != 2)
    {
        fprintf(stderr, "You need to provide a sudoku data file!\nUsage: ./sudoku <filename>\n");
        return -1;
    }
    FILE *filePointer;
    char buffer[100];
    filePointer = fopen(argv[1], "r");
    if (filePointer == NULL)
    {
        fprintf(stderr, "Can't Find File!\n");
        return 1;
    }
    int row = 0;
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL)
    {
        // printf("%s", buffer);
        int length = strlen(buffer);
        token = strtok(buffer, ",");
        for (int col = 0; token != NULL; col++)
        {
            sudoku[row][col] = atoi(token);
            token = strtok(NULL, ",");
        }
        row++;
    }
    fclose(filePointer);
    
    parameters *checksquare1 = (parameters *)malloc(sizeof(parameters));
    checksquare1->row = 0;
    checksquare1->column = 0;
    parameters *checksquare2 = (parameters *)malloc(sizeof(parameters));
    checksquare2->row = 0;
    checksquare2->column = 3;
    parameters *checksquare3 = (parameters *)malloc(sizeof(parameters));
    checksquare3->row = 0;
    checksquare3->column = 6;
    parameters *checksquare4 = (parameters *)malloc(sizeof(parameters));
    checksquare4->row = 3;
    checksquare4->column = 0;
    parameters *checksquare5 = (parameters *)malloc(sizeof(parameters));
    checksquare5->row = 3;
    checksquare5->column = 3;
    parameters *checksquare6 = (parameters *)malloc(sizeof(parameters));
    checksquare6->row = 3;
    checksquare6->column = 6;
    parameters *checksquare7 = (parameters *)malloc(sizeof(parameters));
    checksquare7->row = 6;
    checksquare7->column = 0;
    parameters *checksquare8 = (parameters *)malloc(sizeof(parameters));
    checksquare8->row = 6;
    checksquare8->column = 3;
    parameters *checksquare9 = (parameters *)malloc(sizeof(parameters));
    checksquare9->row = 6;
    checksquare9->column = 6;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&tid[0], &attr, checkRow, NULL);
    pthread_create(&tid[1], &attr, checkSquare, checksquare1);
    pthread_create(&tid[2], &attr, checkSquare, checksquare2);
    pthread_create(&tid[3], &attr, checkSquare, checksquare3);
    pthread_create(&tid[4], &attr, checkSquare, checksquare4);
    pthread_create(&tid[5], &attr, checkSquare, checksquare5);
    pthread_create(&tid[6], &attr, checkSquare, checksquare6);
    pthread_create(&tid[7], &attr, checkSquare, checksquare7);
    pthread_create(&tid[8], &attr, checkSquare, checksquare8);
    pthread_create(&tid[9], &attr, checkSquare, checksquare9);
    pthread_create(&tid[10], &attr, checkColumn, NULL);

    for (int i = 0; i < 11; i++)
    {
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    if (isValid == 11)
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
    for (int i = 0; i < 9; i++)
    {
        int check[10] = {0};
        for (int j = 0; j < 9; j++)
        {
            if (check[sudoku[i][j]] == 1)
            {
                // printf("Row %d is invalid!\n", i);
                pthread_exit(0);
            }
            check[sudoku[i][j]] = 1;
        }
    }
    isValid += 1;
    pthread_exit(0);
}

void *checkColumn(void *param)
{
    for (int i = 0; i < 9; i++)
    {
        int check[10] = {0};
        for (int j = 0; j < 9; j++)
        {
            if (check[sudoku[j][i]] == 1)
            {
                // printf("Column %d is invalid!\n", i);
                pthread_exit(0);
            }
            check[sudoku[j][i]] = 1;
        }
    }
    isValid += 1;
    pthread_exit(0);
}

void *checkSquare(void *param)
{
    int row = ((parameters *)param)->row;
    int column = ((parameters *)param)->column;
    int check[10] = {0};
    for (int i = row; i < row + 3; i++)
    {
        for (int j = column; j < column + 3; j++)
        {
            if (check[sudoku[i][j]] == 1)
            {
                // printf("Square %d %d is invalid!\n", row, column);
                pthread_exit(0);
            }
            check[sudoku[i][j]] = 1;
        }
    }
    isValid += 1;
    pthread_exit(0);
}