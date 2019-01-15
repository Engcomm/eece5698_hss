/**
 * Implementation of AES and RSA Performance testing.
 * Lab module 1 for Lab 1: AES and RSA
 * Created by: Arjun Gupta
 * Jan 11, 2019
 * 
 **/

#include "aes_rsa_performance.h"

#define BILLION 1000000000L


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
    struct timespec begin, end;
    long long unsigned int diff;
    // 2. encrypt data using AES_encrypt and time each
    for (int k = 0; k < 1000000; k++) {
        clock_gettime(CLOCK_MONOTONIC, &begin);
        AES_encrypt((const unsigned char *) text, cipher_text, (const AES_KEY *) &aes_key);
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
        
        // printf("Time elapsed = %llu nanoseconds\n", diff);
        
        // 3. Write each set of AES and RSA data to CSV file
        fprintf(fp, "%llu\n", diff);
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

    struct timespec begin, end;
    long long unsigned int diff;
    int text_len = (int) strlen(text);
    // 2. encrypt data using AES_encrypt and time each
    for (int k = 0; k < 1000000; k++) {
        clock_gettime(CLOCK_MONOTONIC, &begin);
        RSA_public_encrypt(text_len, text, encrypted, rsa, RSA_PKCS1_PADDING);
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        diff = BILLION * (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
        // usleep(100);
        // printf("Time elapsed = %llu nanoseconds\n", diff);
    
        // 3. Write each set of AES and RSA data to CSV file
        fprintf(fp, "%llu\n", diff);
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