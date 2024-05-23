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
int frameBitSize, totalFrames, frameSize, pageBitSize, pageSize, pysicalMemorySize, freeFrameCount;

// 初始化数据结构
void init() {
    for (int i = 0; i < TLB_SIZE; i++)
        tlb[i].pageNumber = -1;
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        pageTable[i] = -1;
        validInvalid[i] = 0;
    }
    for (int i = 0; i < PHYSICAL_FRAMES; i++)
        freeFrameList[i] = i; // 初始化空闲帧列表
    freeFrameCount = PHYSICAL_FRAMES;
}

// 检查TLB
int checkTLB(int pageNumber) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].pageNumber == pageNumber) {
            return tlb[i].frameNumber;
        }
    }
    return -1;
}

// 更新TLB（FIFO策略）
void updateTLB(int pageNumber, int frameNumber) { 
    static int tlbIndex = 0;
    tlb[tlbIndex].pageNumber = pageNumber;
    tlb[tlbIndex].frameNumber = frameNumber;
    tlbIndex = (tlbIndex + 1) % TLB_SIZE;
}

// 检查页表
int checkPageTable(int pageNumber) {
    if (validInvalid[pageNumber] == 1) {
        return pageTable[pageNumber];
    }
    return -1;
}

// 更新页表
void updatePageTable(int pageNumber, int frameNumber) {
    pageTable[pageNumber] = frameNumber;
    validInvalid[pageNumber] = 1;
}

// 从后备存储加载页面
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

// 加载页面到物理内存
void loadPage(int pageNumber) {
    int frameNumber;
    if (freeFrameCount > 0) {
        frameNumber = freeFrameList[--freeFrameCount];
    } else {
        frameNumber = replacePage(pageNumber);
    }
    loadPageFromBackingStore(pageNumber, frameNumber);
    updatePageTable(pageNumber, frameNumber);
    updateTLB(pageNumber, frameNumber);
}

// 页面替换（FIFO策略）
int replacePage(int pageNumber) {
    static int nextFrameToReplace = 0;
    int frameNumber = nextFrameToReplace;
    nextFrameToReplace = (nextFrameToReplace + 1) % PHYSICAL_FRAMES;

    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        if (pageTable[i] == frameNumber) {
            validInvalid[i] = 0;
            break;
        }
    }
    return frameNumber;
}

// 地址翻译
void translateAddress(int logicalAddress) {
    int pageNumber = (logicalAddress >> frameBitSize) & (pageSize - 1); // 页号
    int offset = logicalAddress & (frameSize - 1); // 页内偏移
    int frameNumber = checkTLB(pageNumber); // 检查TLB
    if (frameNumber != -1) {
        int physicalAddress = frameNumber * frameSize + offset;
        char data = physicalMemory[physicalAddress];
        printf("[TLB] %d -> %d: %d\n", logicalAddress, physicalAddress, data);
        return;
    } 
    frameNumber = checkPageTable(pageNumber);
    if (frameNumber != -1) {
        int physicalAddress = frameNumber * frameSize + offset;
        char data = physicalMemory[physicalAddress];
        printf("[Page Table] %d -> %d: %d\n", logicalAddress, physicalAddress, data);
        updateTLB(pageNumber, frameNumber);
        return;
    } 
    // 页故障
    loadPage(pageNumber);
    frameNumber = checkPageTable(pageNumber); // 加载完成后重新检查页表
    int physicalAddress = frameNumber * frameSize + offset;
    char data = physicalMemory[physicalAddress];
    printf("[Load Page] %d -> %d: %d\n", logicalAddress, physicalAddress, data);
}

int check_input(int frameSize, int totalNumber){
    pysicalMemorySize = frameSize * totalNumber;
    return (pysicalMemorySize < MAX_PHYSICAL_MEMORY_SIZE && (pysicalMemorySize & (pysicalMemorySize - 1)) == 0);
}

int main() {
    do{
        printf("Enter frame size (in bits): ");
        scanf("%d", &frameBitSize); 
        printf("Enter total number of frames: ");
        scanf("%d", &totalFrames); //物理内存中的总帧数
        frameSize = 1 << frameBitSize;
    }
    while (!check_input(frameSize, totalFrames));
    pysicalMemorySize = frameSize * totalFrames;
    pageBitSize = 16 - frameBitSize;
    pageSize = 1 << pageBitSize;
    init();

    // 示例地址翻译
    translateAddress(16916); // 示例：逻辑地址16916
    translateAddress(25412); // 示例：逻辑地址25412

    return 0;
}