#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT "12000"
#define SECRET_SIZE 16
#define SIZE 8192

unsigned char aes_key[16] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
    0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff 
};
// unsigned char aes_key[16] = {
//     0x00, 0x01, 0x02, 0x03,
//     0x04, 0x05, 0x06, 0x07,
//     0x08, 0x09, 0x0a, 0x0b,
//     0x0c, 0x0d, 0x0e, 0x1f
// };

// unsigned char aes_key[16] = "0123456789ABCDEF"

char public_key[] = "-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy0037LjaVriXhkk7rkc6\n"
"A79ZI8kh8hKZsIkVugFT7zEvnrxmo5ObI0xtZgd2LbuO3bf1q//byXsZgFQg2Qw7\n"
"HavjnQ/SZQJ+Y0pMsYglLog2llH5+DQtHo9anRLI+B4vVXkOM0BgwTp9KHAdmloM\n"
"XKSQ0aII4aSeAGT36HVKiHIrVq0K2hUUTv/r8uR+j/tEM/ULw1i5oWGHGYI4i+rB\n"
"z66glpBscEPQu2z5rCv+8Fb9kQRo/xQ1QUtPShzYbFBu1qGWqYQeeE1gwPWp6WRh\n"
"URJ2tFYI5tpauf4uL8r775YLPy80Gre0Vq+Vf8IQ/pKvpBnLDF6HDT4amimY1QLv\n"
"xQIDAQAB\n"
"-----END PUBLIC KEY-----\n";

int connect_serv(int *sock_fd, struct addrinfo *serv_addr);

int read_bytes(const int sock_fd, char * buff, const size_t size);
int write_bytes(const int sock_fd, const char *buff, const size_t size);
RSA* createRSA(unsigned char *key);
int public_encrypt(unsigned char *data, int data_len, unsigned char *key, unsigned char *encrypted);

int main();