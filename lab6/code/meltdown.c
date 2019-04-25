#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>
#include <time.h>

#define MEMORY_PAGE_SIZE 4096

uint8_t *userArray; 
static size_t reload_threshold; 

void clflush(volatile void* Tx) {
    asm volatile("lfence;clflush (%0) \n" :: "c" (Tx));
}

static __inline__ uint64_t timer_start (void) {
        unsigned cycles_low, cycles_high;
        asm volatile ("CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                    "%rax", "%rbx", "%rcx", "%rdx");
        return ((uint64_t)cycles_high << 32) | cycles_low;
}

static __inline__ uint64_t timer_stop (void) {
        unsigned cycles_low, cycles_high;
        asm volatile("RDTSCP\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t"
                    "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax",
                    "%rbx", "%rcx", "%rdx");
        return ((uint64_t)cycles_high << 32) | cycles_low;
}

static __inline__ void maccess(void *p) {
  asm volatile("movq (%0), %%rax\n" : : "c"(p) : "rax");
}


uint32_t reload(void *target)
{
    uint64_t t1, t2;
    t1 = timer_start();
    maccess(target);
    t2 = timer_stop();
    return t2 - t1;
}

int obtainData(int data)
{
    for (int k = 0; k < 256; k++)
    {
        clflush((void*) (userArray + (k * MEMORY_PAGE_SIZE)));
    }

    maccess((void*) (userArray + (data * MEMORY_PAGE_SIZE)));
    uint32_t timing = 0;
    uint8_t sniffedData = 0;
    for (int k = 0; k < 256; k++) 
    {  
        timing = reload((void*) (userArray + (k * MEMORY_PAGE_SIZE)));
        if (timing < reload_threshold)
        {
            sniffedData = k;
            break;
        }
    }
    if (!sniffedData)
    {
    #ifdef DEBUG 
        printf("Unable to find valid data.\n");
    #endif
        return -1;
    }
    return sniffedData;
}

void setupCovertChannel()
{   
    size_t mallocSize = MEMORY_PAGE_SIZE * sizeof(uint8_t) * 256;
    userArray = malloc(mallocSize);
    memset(userArray, 1, mallocSize);
    srand(time(NULL));
    uint8_t data;
    int sniffedData;
    int correctCount = 0;
    for (int k = 0; k < 100000; k++) 
    {
        data = rand() % 256;
        if ((sniffedData = obtainData(data)) != data)
        {
            continue;
        }
    #ifdef DEBUG
        printf("Matched byte value is %d\n", sniffedData);
    #endif//DEBUG    
        correctCount++;
    }
    printf("Matched %d correct bytes using covert channel\n", correctCount);
    
}

void findThreshold()
{
    size_t reloadTime = 0, flushReloadTime = 0, i, numTests=100000;
    void* addr = malloc(10);
    
    size_t *ptr = addr + 8;
    maccess(addr);
    for (i = 0; i < numTests; i++)
    {
        reloadTime += reload((void*) ptr);
    }

    for (i = 0; i < numTests; i++) 
    {
        flushReloadTime += reload((void*) ptr);
        clflush(addr);
    }

    reloadTime /= numTests;
    flushReloadTime /= numTests;
    reload_threshold = (reloadTime*.8) + (flushReloadTime*.2);
// #ifdef DEBUG
    printf("Reload time is %zu\t Flush+Reload time is %zu\n", reloadTime, flushReloadTime);
    printf("Threshold is %zu\n", reload_threshold);
// #endif 
}


int main(int argc, char** argv)
{
    findThreshold();
    setupCovertChannel();
    return 0;
}

