/**
 * Lab 2 - DFA implementation
 * Starter code attributed from 
 * Linked List Forward-Chaining implementation attributed from: https://codereview.stackexchange.com/questions/62753/creating-nodes-for-linked-list-in-a-while-loop-in-c
 * 
 * 
 **/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define ECB 1

#include "aes.h"

typedef uint8_t state_t[4][4];

const uint8_t rsbox[256] = {
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };



static void phex(uint8_t* str);
static void phex_len(uint8_t* str, uint8_t len, uint8_t modamt);
static void test_encrypt_ecb_fault(void);

void get_modified_bytes(uint8_t *cipher, uint8_t *faulty, int size, int *output);
void get_valid_differentials(uint8_t arr[], uint8_t cipher_val, uint8_t faulty_val, int *output);
uint8_t get_small_ro(uint8_t cipher_byte, uint8_t faulty_byte, uint8_t key_byte);
int find_intersection(uint8_t arr[], int size, int val);
uint8_t get_key(uint8_t cipher_byte1, uint8_t faulty_byte1, uint8_t cipher_byte2, uint8_t faulty_byte2, uint8_t dro_byte);


int main(void)
{
    // int exit;

#if defined(AES128)
    printf("\nTesting AES128\n\n");
#endif
    test_encrypt_ecb_fault();

    return 0;
}


/**
 * Prints a string in hex
 * @param str - the string to print 
 **/
static void phex(uint8_t* str)
{

#if defined(AES128)
    uint8_t len = 16;
#endif

    unsigned char i;
    printf(" ");
    for (i = 0; i < len; ++i)
        printf(" %.2X ", str[i]);
    printf("\n");
}

/**
 * Prints a string in hex given a certain length, and specific line deliniations 
 * @param str - the string to print
 * @param len - the length of the string to print
 * @param modamt - every multiple of this number, print a new line
 * 
 **/
static void phex_len(uint8_t* str, uint8_t len, uint8_t modamt)
{
    unsigned char i;
    printf(" ");
    for (i = 0; i < len; ++i){
        if ((i % modamt) == 0) {
            printf("\n");
        }
        printf(" %.2X ", str[i]);
    }
    printf("\n");
}

/**
 * Returns the bytes modified for a specific set of cipher and faulty texts.
 * Stores the result in output.
 * @param cipher     the ciphertext bytestring 
 * @param faulty     the faultytext bytestring
 * @param size       the size of the bytestring
 * @param output     the output that contains the byte positions that were modified
 **/
void get_modified_bytes(uint8_t *cipher, uint8_t *faulty, int size, int *output) {
    int byte_count = 0;
    for (int k = 0; k < size; k++) {
        if (cipher[k] != faulty[k]) {
            output[byte_count] = k;
            byte_count++;
        }
    }
}

/**
 * For an array, finds the index that matches the given val parameter
 * The array represents the Round9 output of AES
 * @param arr       the array that needs to be checked for a value
 * @param size      the size of the array to be checked
 * @param val       the value that needs to be checked for 
 **/ 
int find_intersection(uint8_t arr[], int size, int val) {
    for (int k = 0; k < size; k++) {
        if (arr[k] == val) {
            return k;
        }
    }
    return -1;
}

/**
 * Reverses the AddRoundKey and SubBytes methods from the last round
 * @param cipher_byte   the single cipher byte
 * @param faulty_byte   the single faulty byte  
 * @param key           the key to be XOR'd against, reverses the AddRoundKey
 * @return the result of the reversal process
 **/
uint8_t get_small_ro(uint8_t cipher_byte, uint8_t faulty_byte, uint8_t key_byte) {
    uint8_t cipher_sinv = rsbox[cipher_byte ^ key_byte];
    uint8_t faulty_sinv = rsbox[faulty_byte ^ key_byte];
    return (cipher_sinv ^ faulty_sinv);
}

/**
 * Stores in output the valid fault values that could have been injected during the Round 9 Mix Columns
 * @param arr           the array of potential mix column delta values
 * @param cipher_val    the specific cipher byte
 * @param faulty_val    the specific faulty bye
 * @param output        the output array to store the delta data into
 **/
void get_valid_differentials(uint8_t arr[], uint8_t cipher_val, uint8_t faulty_val, int *output) {

    uint8_t result;
    int index; 
    for (int key = 0; key < 256; key++) {
        result = get_small_ro(cipher_val, faulty_val, key);
        // printf("S^-1(%02X XOR %02X) XOR S^-1(%02X XOR %02X) = %02X\n", cipher_val, key, faulty_val, key, result);
        index = find_intersection(arr, 255, result);
        if (index != -1) {
            output[index] = 1;
        }
    }
}


/**
 * Returns a key given two sets of cipher and faulty bytes, as well as a Round9Output value
 * @param cipher_byte1  the first cipher byte
 * @param faulty_byte1  the first faulty byte
 * @param cipher_byte2  the second cipher byte
 * @param faulty_byte2  the second faulty byte
 * @param dro_byte      the round9 delta output given by the previously obtained mapping and fault values
 * 
 **/
uint8_t get_key(uint8_t cipher_byte1, uint8_t faulty_byte1, 
                uint8_t cipher_byte2, uint8_t faulty_byte2, uint8_t dro_byte) {
    uint8_t key_vals[256] = {0};
    int index = 0;
    uint8_t result;
    for (int key = 0; key < 256; key++) {
        result = get_small_ro(cipher_byte1, faulty_byte1, key);
        if (result == dro_byte) {
            // printf("Key could be %02X\n", key);
            key_vals[index] = key;
            index++;
        }
    }
    for (int j = 0; j < index; j++) {
        result = get_small_ro(cipher_byte2, faulty_byte2, key_vals[j]);
        if (result == dro_byte) {
            return key_vals[j];
        }
    }
    return 0;
}



/**
 * High level function for testing if the DFA method is working
 * 
 */ 
static void test_encrypt_ecb_fault(void)
{
    // Example of more verbose verification


    // 128bit key
    uint8_t key[16] =        { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };

    uint8_t plain_text1[16] = {   (uint8_t) 0x6b, (uint8_t) 0xc1, (uint8_t) 0xbe, (uint8_t) 0xe2,
                                        (uint8_t) 0xae, (uint8_t) 0x2d, (uint8_t) 0x8a, (uint8_t) 0x57,
                                        (uint8_t) 0x30, (uint8_t) 0xc8, (uint8_t) 0x1c, (uint8_t) 0x46,
                                        (uint8_t) 0xf6, (uint8_t) 0x9f, (uint8_t) 0x24, (uint8_t) 0x45 };

    uint8_t plain_text2[16] = {   (uint8_t) 0xe5, (uint8_t) 0x41, (uint8_t) 0xac, (uint8_t) 0x59,
                                        (uint8_t) 0x28, (uint8_t) 0x89, (uint8_t) 0x4d, (uint8_t) 0xc5,
                                        (uint8_t) 0xa0, (uint8_t) 0xab, (uint8_t) 0x8a, (uint8_t) 0xec,
                                        (uint8_t) 0x17, (uint8_t) 0xbe, (uint8_t) 0x37, (uint8_t) 0x58 };


    uint8_t ciphertext1[16];
    uint8_t ciphertext2[16];
    uint8_t faultytext1[16];
    uint8_t faultytext2[16];

    // print text to encrypt, key and IV
    printf("ECB encrypt verbose:\n\n");
    printf("first plain text:\n");
    phex_len(plain_text1, 16, 4);
    printf("\n\n");

    printf("second plain text:\n");
    phex_len(plain_text2, 16, 4);
    printf("\n\n");

    printf("key:\n");
    phex(key);
    printf("\n");

    // print the resulting cipher as 4 x 16 byte strings
    
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);


    // uint8_t byte_pos = 4;
    uint8_t byte_col = 3;
    uint8_t byte_row = 0;
    // get_modified_bytes(bytes_modified, plain_text_cipher_1, plain_text_);
    // uint8_t mc_delta_input[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
    // struct map_node* DRO_map[4];
    state_t mc_delta_input = {0};
    uint8_t DRO_map[4][255];
    memset(DRO_map, 0, sizeof(DRO_map[0][0]) * 4 * 255);
    for (int k = 1; k < 256; k++) {
        memset(mc_delta_input, 0, sizeof(mc_delta_input[0][0]) * 16);
        mc_delta_input[byte_col][byte_row] = k;
        MixColumns((state_t*) mc_delta_input);
        DRO_map[0][k-1] = mc_delta_input[byte_col][3];
        DRO_map[1][k-1] = mc_delta_input[byte_col][2];
        DRO_map[2][k-1] = mc_delta_input[byte_col][1];
        DRO_map[3][k-1] = mc_delta_input[byte_col][0];
    }

    //Generate random number 
    srand(time(0));

    uint8_t inject = (rand() % 255) + 1;
    // uint8_t inject = 1;

    memcpy(ciphertext1, plain_text1, 16);
    memcpy(faultytext1, plain_text1, 16);
    memcpy(ciphertext2, plain_text2, 16);
    memcpy(faultytext2, plain_text2, 16);

    AES_ECB_encrypt(&ctx, ciphertext1);
    AES_ECB_encrypt(&ctx, ciphertext2);

    AES_ECB_encrypt_fault(&ctx, faultytext1, byte_col, byte_row, inject);
    AES_ECB_encrypt_fault(&ctx, faultytext2, byte_col, byte_row, inject);

    printf("First ciphertext: ");
    phex_len(ciphertext1, 16, 16);
    printf("\n");

    printf("First faultytext: ");
    phex_len(faultytext1, 16, 16);
    printf("\n");

    printf("Second ciphertext: ");
    phex_len(ciphertext2, 16, 16);
    printf("\n");


    printf("Second faultytext: ");
    phex_len(faultytext2, 16, 16);
    printf("\n");

    int modified_bytes[4] = {0};
    get_modified_bytes(ciphertext1, faultytext1, 16, modified_bytes);

    printf("Modified bytes are: ");
    for (int k = 0; k < 4; k++) {
        printf(" %d ", modified_bytes[k]);
    }
    printf("\n");

    //Find the sbox inverse for each of the modified columns
    uint8_t modified_byte_pos;
    uint8_t cipher_byte;
    uint8_t faulty_byte;
    int dro_index_map[8][255];
    memset(dro_index_map, 0, sizeof(dro_index_map[0][0]) * 8 * 255);

    for (int k = 0; k < 8; k++) {
        modified_byte_pos = modified_bytes[(k % 4)];
        if (k < 4) {
            cipher_byte = ciphertext1[modified_byte_pos];
            faulty_byte = faultytext1[modified_byte_pos];
        }
        else {
            cipher_byte = ciphertext2[modified_byte_pos];
            faulty_byte = faultytext2[modified_byte_pos];
        }
        // printf("K is %d\n", k);
        get_valid_differentials(DRO_map[(k % 4)], cipher_byte, faulty_byte, dro_index_map[k]);
    }


    int valid_bytes[256];
    for (int k = 1; k < 256; k++) {
        valid_bytes[k-1] = 1;
    }
    int valid_bytes_size = sizeof(valid_bytes) / sizeof(valid_bytes[0]);
    int valid_count = valid_bytes_size;
    for (int k = 0; k < valid_bytes_size; k++) {
        if (!valid_count) {
            valid_count--;
        }
    }
    printf("\nInitial possible fault space: %d\n\n", --valid_count);
    for (int k = 0; k < 8; k++) {
        if (k == 4) {
            printf("\nAnalyzing second cipher and faulty text values...\n\n");
        }
        for (int j = 0; j < 255; j++) {
            if (valid_bytes[j]) {
                if (!dro_index_map[k][j]) {
                    valid_bytes[j] = 0;
                    valid_count--;
                }
            }
        }
        printf("After round %d, the fault space is %d\n", k+1, valid_count);
    }
    int fault_index = -1;
    for (int k = 0; k < 256; k++) {
        if (valid_bytes[k]) {
            fault_index = k;
        }
    }

    printf("\nChecking if calculated fault value matches injected...\n");
    printf("Calculated value: %d\tInjected Value: %d\n", fault_index+1, inject);
    if ((fault_index + 1) == inject) {
        printf("Successful in calculating the fault value.\n\n");
    }
    else {
        printf("Unsuccessful in calclating the fault value. Exiting..\n:");
        return;
    }
    
    if (fault_index == -1) {
        printf("How did you even get here...? Something went wrong.\n");
        return;
    }

    uint8_t common_index_round_out[4];
    
    printf("Fault is: ");
    for (int k = 0; k < 4; k++) {
        common_index_round_out[k] = DRO_map[k][fault_index];
        printf("%02X ", common_index_round_out[k]);
    }
    printf("\n");

    uint8_t keys[4];
    uint8_t key_found;
    uint8_t cipher_byte1, cipher_byte2;
    uint8_t faulty_byte1, faulty_byte2;
    uint8_t dro_byte; 
    for (int k = 0; k < 4; k++) {
        modified_byte_pos = modified_bytes[k];
        cipher_byte1 = ciphertext1[modified_byte_pos];
        cipher_byte2 = ciphertext2[modified_byte_pos];
        faulty_byte1 = faultytext1[modified_byte_pos];
        faulty_byte2 = faultytext2[modified_byte_pos];
        dro_byte = common_index_round_out[k];
        key_found = get_key(cipher_byte1, faulty_byte1, cipher_byte2, faulty_byte2, dro_byte);
        keys[k] = key_found;
    }

    printf("The key is: ");
    for (int k = 0; k < 4; k++) {
        printf(" %02X ", keys[k]);
    }
    printf("\n");
}
