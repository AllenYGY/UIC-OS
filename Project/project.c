
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pageNumber;
    int frameNumber;
} entry;

int frameSize;
int totalNumber;
int bitsForPageNo;
int pageTableSize;

int indexOfTLB = 0;
int hit = 0;
int fault = 0;
int count = 0;
int numOfFreeFrames = 0;

char values[256][65536];

int pageTable[65536];//An array which stores the valid-invalid i of each page
entry TLB[16];

void init()
{
    for (int i = 0; i < 16; i++)
    {
        TLB[i].pageNumber = -1;
        TLB[i].frameNumber = -1;
    }
    
    for (int i = 0; i < 65536; i++)
    {
        pageTable[i] = -1;
    }
}

void calculate_bitsForPageNo(int frameSize)
{
    bitsForPageNo = 16 - frameSize;
    pageTableSize = 1 << bitsForPageNo;
}

int check_input(int frameSize, int totalNumber) 
{
    int size = 1 << frameSize;
    int physical_memory = size * totalNumber;
    return (physical_memory <= 65536 && (physical_memory & (physical_memory - 1)) == 0);
}

int checkTLB(int pageNumber)
{
    for (int i = 0; i < 16; i++)
    {
        if (TLB[i].pageNumber == pageNumber)
        {
            hit++;
            return TLB[i].frameNumber;
        }
    }
    return -1;
}

void updateTLB(int pageNumber, int frameNumber)
{
    TLB[indexOfTLB].pageNumber = pageNumber;
    TLB[indexOfTLB].frameNumber = frameNumber;
    indexOfTLB = (indexOfTLB + 1) % 16;
}

int checkPageTable(int pageNumber)
{
    return pageTable[pageNumber];
}

void updatePageTable(int pageNumber, int frameNumber)
{
    pageTable[pageNumber] = frameNumber;
}

void loadPage(int pageNumber)
{
    FILE *backing_store = fopen("backingstore.bin", "rb");
    if (!backing_store) 
    {
        printf("Error: fail with opening backing store\n");
        exit(EXIT_FAILURE);
    }

    fseek(backing_store, pageNumber * (1 << frameSize), SEEK_SET);
    fread(values[numOfFreeFrames], sizeof(char), (1 << frameSize), backing_store);
    fclose(backing_store);

    updatePageTable(pageNumber, numOfFreeFrames);
    updateTLB(pageNumber, numOfFreeFrames);

    printf("    [Load Page] Page %d -> Frame %d\n", pageNumber, numOfFreeFrames);
    numOfFreeFrames++;
    fault++;
}

void replacePage(int pageNumber)
{
    //static int replaceIndex = 0;
    //int index = replaceIndex % totalNumber;
    int index = numOfFreeFrames % totalNumber;
    int replacedPage = -1;
    for (int i = 0; i < pageTableSize; i++) 
    {
        if (pageTable[i] == index) 
        {
            replacedPage = i;
            break;
        }
    }

    pageTable[replacedPage] = -1;
    // if (replacedPage != -1) 
    // {
    //     for (int i = 0; i < 16; i++) 
    //     {
    //         if (TLB[i].pageNumber == replacedPage) 
    //         {
    //             TLB[i].pageNumber = -1;
    //             TLB[i].frameNumber = -1;
    //             break;
    //         }
    //     }
    // }
    
    FILE *backing_store = fopen("backingstore.bin", "rb");
    if (!backing_store) {
        printf("Error: failed to open backing store\n");
        exit(EXIT_FAILURE);
    }

    fseek(backing_store, pageNumber * (1 << frameSize), SEEK_SET);
    fread(values[index], sizeof(char), (1 << frameSize), backing_store);
    fclose(backing_store);

    updatePageTable(pageNumber, index);
    updateTLB(pageNumber, index);

    printf("    [Replace Page] Frame %d: Page %d -> Page %d\n", index, replacedPage, pageNumber);
    fault++;
    numOfFreeFrames++;
    //replaceIndex++;
    //index = (index + 1) % totalNumber;
}

void translate(int address)
{
    int pageNumber = (address >> (16 - bitsForPageNo));
    int offset = address & ((1 << (16 - bitsForPageNo)) - 1);
    count++;
    int frameNumber = checkTLB(pageNumber);
    if (frameNumber != -1) 
    {
        printf("[TLB] (LA) %d -> (PA) %d: %c\n", address, (frameNumber << (16 - bitsForPageNo)) | offset, values[frameNumber][offset]);
        return;
    }
    
    frameNumber = checkPageTable(pageNumber);
    if (frameNumber != -1) 
    {
        printf("[Page Table] (LA) %d -> (PA) %d: %c\n", address, (frameNumber << (16 - bitsForPageNo)) | offset, values[frameNumber][offset]);
        updateTLB(pageNumber, frameNumber);
        return;
    }
    
    if (numOfFreeFrames < totalNumber) 
    {
        loadPage(pageNumber);
    } else 
    {
        replacePage(pageNumber);
    }
    
    frameNumber = checkTLB(pageNumber);
    if (frameNumber != -1) 
    {
        printf("[TLB] (LA) %d -> (PA) %d: %c\n", address, (frameNumber << (16 - bitsForPageNo)) | offset, values[frameNumber][offset]);
        return;
    }
}

void generateStat()
{
    FILE *stat_file = fopen("stat.txt", "w");
    if (!stat_file) 
    {
        printf("Error: fail with opening backing store\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stat_file, "page-fault rate: %.2f\n", (double)fault / count);
    fprintf(stat_file, "TLB hit rate: %.2f\n", (double)hit / count);
    fprintf(stat_file, "Memory image:\n");

    // for (int i = 0; i < totalNumber; i++) 
    // {
    //     if (i % 16 == 0) 
    //     {
    //         fprintf(stat_file, "Frame %d ~ Frame %d: ", i, i + 15);
    //     }
    //     fprintf(stat_file, "%d ", pageTable[i]);
    //     if (i % 16 == 15) 
    //     {
    //         fprintf(stat_file, "\n");
    //     }
    // }

    for (int i = 0; i < totalNumber / 16; i++) 
    {
        fprintf(stat_file, "Frame %d ~ Frame %d:", i * 16, (i + 1) * 16 - 1);
        for (int j = 0; j < 16; j++) 
        {
            int frameNumber = i * 16 + j;
            int pageNumber = -1;
            for (int j = 0; j < pageTableSize; j++) 
            {
                if (pageTable[j] == frameNumber) 
                {
                    pageNumber = j;
                    break;
                }
            }
            fprintf(stat_file, " %d", pageNumber);
        }
        fprintf(stat_file, "\n");
    }

    fclose(stat_file);
}

int main()
{ 
    printf("Please input the frame size((use number of bits to represent): ");
    scanf("%d", &frameSize);
    printf("Please input the total number of frames in physical memory: ");
    scanf("%d", &totalNumber);
    while (!check_input(frameSize, totalNumber))
    {
        printf("Warning! Your input is invalid! Please input again.\n");
        printf("Please input the frame size(use number of bits to represent): ");
        scanf("%d", &frameSize);
        printf("Please input the total number of frames in physical memory: ");
        scanf("%d", &totalNumber);
    }
    init();
    calculate_bitsForPageNo(frameSize);
    
    FILE *addresses_file = fopen("addresses.txt", "r");
    if (!addresses_file) 
    {
        printf("Error: fail with opening addresses file\n");
        exit(EXIT_FAILURE);
    }
    int address;
    while (fscanf(addresses_file, "%d", &address) != EOF) 
    {
        translate(address);
    }
    fclose(addresses_file);

    generateStat();

    return 0;
}
