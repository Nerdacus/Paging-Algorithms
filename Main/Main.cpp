#include <stdio.h>

/* Logical defines. */
#define TRUE 1
#define FALSE 0

/* Page defines. */
#define PAGE_SIZE 100
#define MAX_FRAMES 5

/* Paging algorithm modes. */
#define FIFO 1
#define LRU 2
#define OPT 3

typedef struct {
    int pageNum;
    int usage;
    int lastUsed;
    int timeStamp;
} pageData;

int adds[100];
pageData frames[MAX_FRAMES];

int readAddressStream(const char *filename);
void showAdds(int numAdds);
int pageReplace(int numAdds, int mode);
int searchFrameTable(int pageNum, int nFrames);
void showFrameTable(int nFrames);
int getIndexOfOldestPage(int nFrames);
int getIndexOfLRUPage(int nFrames);
int getIndexOfBeladyPage(int nFrames, int numAdds);

int main(void) {
    int numAdds;
    int pageFaults;

    /* Send message to user. */
    printf("Hello TV Land! \n");

    /* Read the incoming address stream from the input file. */
    numAdds = readAddressStream("address.txt");
    printf("numAdds = %d \n", numAdds);

    /* Show the addresses to the user. */
    showAdds(numAdds);

    /* Implement the FIFO page replacement algorithm. */
    printf("Page replacement (FIFO) \n");
    pageFaults = pageReplace(numAdds, FIFO);
    printf("pageFaults (FIFO) = %d \n", pageFaults);

    /* Implement the LRU page replacement algorithm. */
    printf("\n");
    printf("Page replacement (LRU) \n");
    pageFaults = pageReplace(numAdds, LRU);
    printf("pageFaults (LRU) = %d \n", pageFaults);

    /* Implement Belady's page replacement algorithm. */
    printf("\n");
    printf("Page replacement (Belady's OPT) \n");
    pageFaults = pageReplace(numAdds, OPT);
    printf("pageFaults (Belady's OPT) = %d \n", pageFaults);

    return 0;
}

int readAddressStream(const char *filename) {
    FILE *in;
    int address;
    int j;
    in = fopen(filename, "r");
    j = 0;
    while (fscanf(in, "%d", &address) != EOF) {
        adds[j] = address;
        j++;
    }
    fclose(in);
    return j;
}

void showAdds(int numAdds) {
    int j;
    printf("Address Stream. \n");
    for (j = 0; j < numAdds; j++) {
        printf("%d \n", adds[j]);
    }
}

int pageReplace(int numAdds, int mode) {
    int j;
    int pageNum, offset;
    int frameNum, nFrames;
    int repFrame;
    int pageFaults = 0;

    /* Initialize variables. */
    nFrames = 0;

    /* For each address in the incoming address stream, manage the page table. */
    for (j = 0; j < numAdds; j++) {
        /* Calculate page and offset. */
        pageNum = adds[j] / PAGE_SIZE;
        offset = adds[j] % PAGE_SIZE;

        /* Search frame table to see if page is present in memory. */
        frameNum = searchFrameTable(pageNum, nFrames);

        /* If the page is not found in the page table, add it. */
        if (frameNum == -1) {
            /* If there is room in the table, add the frame. */
            if (nFrames < MAX_FRAMES) {
                frames[nFrames].pageNum = pageNum;
                frames[nFrames].usage = 1;
                frames[nFrames].lastUsed = j;
                nFrames++;
            } else {
                /* Page fault. */
                pageFaults++;

                switch (mode) {
                    case FIFO:
                        /* Find oldest frame. */
                        repFrame = getIndexOfOldestPage(nFrames);
                        break;
                    case LRU:
                        /* Find the least recently used frame. */
                        repFrame = getIndexOfLRUPage(nFrames);
                        break;
                    case OPT:
                        /* Find the frame used furthest in the future. */
                        repFrame = getIndexOfBeladyPage(nFrames, numAdds);
                        break;
                }

                /* Replace the frame. */
                frames[repFrame].pageNum = pageNum;
                frames[repFrame].usage = 1;
                frames[repFrame].lastUsed = j;
            }
        } else {
            /* Frame was found in the table. */
            /* Update the usage count and last time used. */
            frames[frameNum].usage++;
            frames[frameNum].lastUsed = j;
        }

        /* Show the frame table to the user. */
        showFrameTable(nFrames);
    }

    return pageFaults;
}

int searchFrameTable(int pageNum, int nFrames) {
    int j;
    int frameIndex = -1;
    for (j = 0; j < nFrames; j++) {
        if (frames[j].pageNum == pageNum) {
            frameIndex = j;
            break;
        }
    }
    return frameIndex;
}

int getIndexOfOldestPage(int nFrames) {
    int j;
    int oldest = frames[0].lastUsed;
    int oldestIndex = 0;
    for (j = 1; j < nFrames; j++) {
        if (frames[j].lastUsed < oldest) {
            oldest = frames[j].lastUsed;
            oldestIndex = j;
        }
    }
    return oldestIndex;
}

int getIndexOfLRUPage(int nFrames) {
    int j;
    int leastRU = frames[0].lastUsed;
    int lIndex = 0;
    for (j = 1; j < nFrames; j++) {
        if (frames[j].lastUsed < leastRU) {
            leastRU = frames[j].lastUsed;
            lIndex = j;
        }
    }
    return lIndex;
}

int getIndexOfBeladyPage(int nFrames, int numAdds) {
    int j;
    int maxFuture = -1;
    int oIndex = 0;
    for (j = 0; j < nFrames; j++) {
        int k;
        int future = 0;
        for (k = 0; k < numAdds; k++) {  // Iterate over the address stream
            if (frames[j].pageNum == adds[k] / PAGE_SIZE) {
                future = k;
                break;
            }
        }
        if (future > maxFuture) {
            maxFuture = future;
            oIndex = j;
        }
    }
    return oIndex;
}

void showFrameTable(int nFrames) {
    int j;
    printf("Frame Table - ");
    for (j = 0; j < MAX_FRAMES; j++) {
        if (j < nFrames) {
            printf("%d ", frames[j].pageNum);
        } else {
            printf("# ");
        }
    }
    printf("\n");
}
