/**
 * AES and RSA Performance testing.
 * Lab module 1 for Lab 1: AES and RSA
 * Created by: Arjun Gupta
 * Jan 11, 2019
 * 
 **/

#ifndef AES_RSA_PERFORMANCE_H
#define AES_RSA_PERFORMANCE_H

#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>  
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

#define NUM_BITS_AES 128

static const unsigned char key[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
    0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff 
};

char public_key[] = "-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy0037LjaVriXhkk7rkc6\n"
"A79ZI8kh8hKZsIkVugFT7zEvnrxmo5ObI0xtZgd2LbuO3bf1q//byXsZgFQg2Qw7\n"
"HavjnQ/SZQJ+Y0pMsYglLog2llH5+DQtHo9anRLI+B4vVXkOM0BgwTp9KHAdmloM\n"
"XKSQ0aII4aSeAGT36HVKiHIrVq0K2hUUTv/r8uR+j/tEM/ULw1i5oWGHGYI4i+rB\n"
"z66glpBscEPQu2z5rCv+8Fb9kQRo/xQ1QUtPShzYbFBu1qGWqYQeeE1gwPWp6WRh\n"
"URJ2tFYI5tpauf4uL8r775YLPy80Gre0Vq+Vf8IQ/pKvpBnLDF6HDT4amimY1QLv\n"
"xQIDAQAB\n"
"-----END PUBLIC KEY-----\n";

int aes_performance_test();

RSA* createRSA(unsigned char * key);
int RSA_performance_test();

int main();

#endif