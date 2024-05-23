#include<stdio.h>
#include<stdlib.h>

// read spcific row's value from backing store
void readRow(int rowNumber)
{
    FILE *backing_store = fopen("backingstore.bin", "rb");
    if (!backing_store) 
    {
        printf("Error: fail with opening backing store\n");
        exit(EXIT_FAILURE);
    }
    fseek(backing_store, rowNumber, SEEK_SET);
    char value;
    fread(&value, sizeof(char), 1, backing_store);
    printf("    [Read Row] Row %d -> Value %d\n", rowNumber, value);
    fclose(backing_store);
}



