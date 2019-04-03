#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <inttypes.h>

#define MEMORY_PAGE_SIZE 4096

uint8_t *userArray; 

static void maccess(void *userArray, void* target, int numNOPS) {
  asm volatile(
      "mov (%%rcx), %%rax\n"
      ".rept"
      "nop\n"
      ".endr\n"
      "mov $10, %%rax\n"
      "shl $12, %%rax\n"
      "movq (%%rbx, %%rax, 1), %%rbx\n"
      :: "c"(target), "b"(userArray), 
      : "rax"
    );
}