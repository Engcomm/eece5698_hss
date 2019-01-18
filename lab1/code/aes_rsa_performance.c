/**
 * Implementation of AES and RSA Performance testing.
 * Lab module 1 for Lab 1: AES and RSA
 * Created by: Arjun Gupta
 * Jan 11, 2019
 * 
 **/

#include "aes_rsa_performance.h"

#define __STDC_FORMAT_MACROS
#define BILLION 1000000000L

static __inline__ uint64_t timer_start (void) {
    unsigned cycles_low, cycles_high;        
    asm volatile ("CPUID\n\t"                    
        "RDTSC\n\t"                    
        "mov %%edx, %0\n\t"                    
        "mov %%eax, %1\n\t": 
        "=r" (cycles_high), "=r" (cycles_low)::                    
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

int aes_performance_test() {
    FILE *fp;
    fp = fopen("aes.txt", "w");

    AES_KEY aes_key;

    //populate a random 16 byte char 
    unsigned char text[16] = "0123456789ABCDEF";
    unsigned char cipher_text[16];
    // 1. Use user key to create AES_KEY with AES_set_encrypt_key to obtain round key

    if (AES_set_encrypt_key(key, NUM_BITS_AES, &aes_key)) {
        printf("Unable to set the round key");
        return -1;
    }
    // struct timespec begin, end;
    uint64_t begin, end;
    uint64_t diff;
    // 2. encrypt data using AES_encrypt and time each
    for (int k = 0; k < 1000000; k++) {
        // clock_gettime(CLOCK_MONOTONIC, &begin);
        begin = timer_start();
        AES_encrypt((const unsigned char *) text, cipher_text, (const AES_KEY *) &aes_key);
        end = timer_stop();
        // clock_gettime(CLOCK_MONOTONIC, &end);
        // diff = BILLION * (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
        diff = end - begin;
        
        // 3. Write each set of AES and RSA data to CSV file
        fprintf(fp, "%lu\n", diff);
    }
    fclose(fp);
    return 0;
}

RSA* createRSA(unsigned char * key) {
    RSA *rsa;
    BIO *bio = BIO_new_mem_buf((void *) key, (int)strlen(key)); //-1 assumes null termination and uses strlen
    rsa = PEM_read_bio_RSA_PUBKEY(bio, &rsa, NULL, NULL);
    BIO_free(NULL);
    return rsa;
}

int RSA_performance_test() {
    FILE *fp;
    fp = fopen("rsa.txt", "w");
    unsigned char text[16] = "0123456789ABCDEF";

    RSA* rsa = createRSA(public_key);
    unsigned char *encrypted = malloc(RSA_size(rsa));
    uint64_t begin, end;
    uint64_t diff;
    int text_len = (int) strlen(text);
    // 2. encrypt data using AES_encrypt and time each
    for (int k = 0; k < 1000000; k++) {
        begin = timer_start();
        RSA_public_encrypt(text_len, text, encrypted, rsa, RSA_PKCS1_PADDING);
        end = timer_stop();
        
        diff = end - begin;
        // usleep(100);
        // printf("Time elapsed = %lu nanoseconds\n", diff);
    
        // 3. Write each set of AES and RSA data to CSV file
        fprintf(fp, "%lu\n", diff);
    }
    fclose(fp);
    return 0;
}

int main() {
    printf("Starting AES Performance test... \n");
    if (aes_performance_test()) {
        return 1;
    }

    printf("Finished AES Performance test...\n");
    usleep(10000);
    printf("Starting RSA Performance test...\n");
    RSA_performance_test();    
    printf("Finished RSA Performance test...\n");

    return 0;
}