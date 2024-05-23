#include <stdio.h>
#include <stdlib.h>

#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 256 // 8 bits
#define MAX_PHYSICAL_MEMORY_SIZE 65536  // 16 bits
#define FRAME_SIZE 256  //8 bits
#define PHYSICAL_FRAMES (MAX_PHYSICAL_MEMORY_SIZE / FRAME_SIZE)

typedef struct {
    int pageNumber;
    int frameNumber;
} TLBEntry;
TLBEntry tlb[TLB_SIZE];

int pageTable[PAGE_TABLE_SIZE];
int validInvalid[PAGE_TABLE_SIZE];
char physicalMemory[MAX_PHYSICAL_MEMORY_SIZE];
int freeFrameList[PHYSICAL_FRAMES];
int frameBitSize, totalFrames, frameSize, pageBitSize, pageTableSize, pysicalMemorySize, freeFrameCount;


// Initialize TLB, Page Table, Physical Memory
void init() {
    for (int i = 0; i < TLB_SIZE; i++)
        tlb[i].pageNumber = -1;
    for (int i = 0; i < pageTableSize; i++)
        pageTable[i] = -1;
    for (int i = 0; i < pysicalMemorySize; i++)
        physicalMemory[i] = -1;
}
// Check TLB
int checkTLB(int pageNumber)
{
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].pageNumber == pageNumber) {
            return tlb[i].frameNumber;
        }
    }
    return -1;
}
// Update TLB FIFO
void updateTLB(int pageNumber, int frameNumber) { 
    static int tlbIndex = 0;
    tlb[tlbIndex].pageNumber = pageNumber;
    tlb[tlbIndex].frameNumber = frameNumber;
    tlbIndex = (tlbIndex + 1) % TLB_SIZE;
}
// Check Page Table
int checkPageTable(int pageNumber) {
    return pageTable[pageNumber];
}
// Update Page Table
void updatePageTable(int pageNumber, int frameNumber) {
    pageTable[pageNumber] = frameNumber;
}

void loadPageFromBackingStore(int pageNumber, int frameNumber) {
    FILE *backingStore = fopen("backingstore.bin", "rb");
    if (backingStore == NULL) {
        fprintf(stderr, "Error opening backing store.\n");
        exit(1);
    }
    fseek(backingStore, pageNumber * FRAME_SIZE, SEEK_SET);
    fread(&physicalMemory[frameNumber * FRAME_SIZE], sizeof(char), FRAME_SIZE, backingStore);
    fclose(backingStore);
}
// Replace Page FIFO
int replacePage(int pageNumber) {
    static int nextFrameToReplace = 0;
    int frameNumber = nextFrameToReplace;
    nextFrameToReplace = (nextFrameToReplace + 1) % totalFrames;
    for (int i = 0; i < pageTableSize; i++) {
        if (pageTable[i] == frameNumber) {
            validInvalid[i] = 0;
            pageTable[i] = -1;
            break;
        }
    }
    loadPageFromBackingStore(pageNumber, frameNumber);
    updatePageTable(pageNumber, frameNumber);
    updateTLB(pageNumber, frameNumber);
    printf("    [Replace page] Frame frame no.: Page replaced page no.%d -> Page loaded page no.%d\n", frameNumber, pageNumber);

    return frameNumber;
}

int loadPage(int pageNumber) {
    int frameNumber = freeFrameList[--freeFrameCount];
    loadPageFromBackingStore(pageNumber, frameNumber);

    return frameNumber;
}

void translateAddress(int logicalAddress) {
    // The first pageBitSize bit is the page number, and the second frameBitSize bit is an in-page offset
    int pageNumber = (logicalAddress >> frameBitSize) & (pageTableSize - 1); // Page Number
    int offset = logicalAddress & (frameSize - 1); // Offset
    int frameNumber = checkTLB(pageNumber); // Check TLB
    if (frameNumber != -1) {
        int physicalAddress = frameNumber * frameSize + offset;
        char data = physicalMemory[physicalAddress];
        printf("[TLB] (LA) %d -> (PA) %d: %d\n", logicalAddress, physicalAddress, data);
        return ;
    } 
    frameNumber = checkPageTable(pageNumber);
    if (frameNumber != -1) {
            int physicalAddress = frameNumber * frameSize + offset;
            char data = physicalMemory[physicalAddress];
            printf("[Page Table] (LA) %d -> (PA) %d: %d\n", logicalAddress, physicalAddress, data);
            updateTLB(pageNumber, frameNumber);
            return ;
    } 

    if (freeFrameCount > 0) { // Load Page
        frameNumber=loadPage(pageNumber);
        printf("    [Load page] Page loaded page no.%d -> Frame frame no.%d\n", pageNumber, frameNumber * frameSize + offset);
    } else {
        frameNumber=replacePage(pageNumber);
    } 
    updatePageTable(pageNumber, frameNumber);
    updateTLB(pageNumber, frameNumber);
}

int check_input(int frameSize, int totalNumber){
    pysicalMemorySize = frameSize * totalNumber;
    return (pysicalMemorySize < 65536 && (pysicalMemorySize & (pysicalMemorySize - 1)) == 0);
}

// Read address.txt and translate the logical address
void readAddress() {
    FILE *addressFile = fopen("addresses.txt", "r");
    if (addressFile == NULL) {
        fprintf(stderr, "Error opening addresses file.\n");
        exit(1);
    }
    int logicalAddress;
    while (fscanf(addressFile, "%d", &logicalAddress) != EOF) {
        translateAddress(logicalAddress);
    }
    fclose(addressFile);
}

int main() {
    do{
        printf("Enter frame size (in bits): ");
        scanf("%d", &frameBitSize); 
        printf("Enter total number of frames: ");
        scanf("%d", &totalFrames); //total number of frames in physical memory
        frameSize = 1 << frameBitSize;
        printf("Physical memory size: %d bytes\n", frameSize * totalFrames);
    }
    while (!check_input(frameSize, totalFrames));
    pysicalMemorySize = frameSize * totalFrames;
    // printf("Physical memory size: %d bytes\n", pysicalMemorySize);
    pageBitSize = 16 - frameBitSize; pageTableSize = 1 << pageBitSize;
    freeFrameCount = totalFrames;
    init();
    readAddress();
    return 0;
}