#include "aes_rsa_comms.h"

/**
 * Set a read timeout.
 *
 * @param sk Socket.
 * @return True if successful.
 */
static int SetReadTimeout(const int sk) {
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  if (setsockopt(sk, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    printf("unable to set read timeout\n");
    return 0;
  }
  return 1;
}

int read_bytes(const int sock_fd, char *buff, const size_t size) {
    char *ptr = buff;
    while (ptr < buff + size) {
        if (!SetReadTimeout(sock_fd)){
            return -1;
        }
        int ret = recv(sock_fd, ptr, ptr - buff + size, 0);

        if (ret < 0) {
            fprintf(stderr, "Unable to receive on socket\n");
            return -1;
        }
        // if (ret > 0) {
        //     printf("Value is \n%d\n", ptr);
        // }
        ptr += ret;
    }
    return 0;
}

int connect_serv(int *sock_fd, struct addrinfo *serv_addr) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int err;
    char buf[SIZE];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; //Defined using same protocol as in server soure 
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    if ((err = getaddrinfo("ecehss.coe.neu.edu", PORT, &hints, &result)) != 0 ) {
        fprintf(stderr, "getaddrinfo %s\n", gai_strerror(err));
        return -1;
    }
    
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if ((*sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1) {
            continue;
        }
        if (connect(*sock_fd, rp->ai_addr, rp->ai_addrlen) != -1) {
            serv_addr = rp;
            break; //successfully connected
        }
        close(*sock_fd); //close this file descriptor.
    }
    return -1;
}

RSA* createRSA(unsigned char* key) {
    RSA *rsa = NULL;
    BIO *bio;
    // bio = BIO_new_mem_buf(key, -1) /**(int) strlen(key)**/;
    // rsa = PEM_read_bio_RSA_PUBKEY(bio, &rsa, NULL, NULL);
    
    //-1 assumes null termination and uses strlen

    if (!(bio = BIO_new_mem_buf(key,(int) strlen(key)))) {
        printf("Error in creating the BIO for the public key\n");
        return NULL;
    } 
    if (!(rsa = PEM_read_bio_RSA_PUBKEY(bio, &rsa, NULL, NULL))) {
        printf("Error in instantiating the RSA!\n");
        return NULL;
    }
    BIO_free(NULL);
    return rsa;
}

int main() {
    int sock_fd;
    struct addrinfo *serv_addr;
    connect_serv(&sock_fd, serv_addr);

    int size;
    char pub_key[SIZE];

    int recvd = recv(sock_fd, &size, sizeof(size), 0);
    read_bytes(sock_fd, pub_key, size);
    // printf("Buffer key is %s", buff);
    char aes_key_encrypted[SIZE];
    // int result = public_encrypt(aes_key, sizeof(buff), buff, aes_key_encrypted);
    RSA* rsa = createRSA(pub_key);
    unsigned char* encrypted_aes_key = malloc(RSA_size(rsa));
    int encoded_size = RSA_public_encrypt(16/**(int) strlen((char*) aes_key)*/, aes_key, encrypted_aes_key, rsa, RSA_PKCS1_PADDING);
    
    // //encrypted_aes_key now holds this client's AES key using the recevied public key
    int sent = 0;
    if ( (sent = send(sock_fd, &encoded_size, sizeof(int), 0)) < 0) {
        printf("Unable to send encrypted AES key size...\n");
        return -1;
    }

    if ( (sent = send(sock_fd, encrypted_aes_key, encoded_size, 0)) < 0 ) {
        printf("Unable to send encrypted AES key... \n");
        return -1;
    }

    AES_KEY secret;

    AES_set_decrypt_key(aes_key, 128, &secret);
    unsigned char serv_secret_encrypted[1024];

    if ((recvd = recv(sock_fd, serv_secret_encrypted, 16, 0)) < 0) {
        printf("Unable to receive secret...\n");
        return -1;
    }

    unsigned char serv_secret_decrypted[1024];
    FILE *fp = fopen("secret.txt", "w+"); //open a new file if necessary
    if (fp == NULL ) {
        fp = fopen("secret.txt", "w");
    }
    AES_decrypt(serv_secret_encrypted, serv_secret_decrypted, &secret);
    for (int k = 0; k < 16; k++) {
        fprintf(fp, "%02X ", serv_secret_decrypted[k]);
    }

    free (encrypted_aes_key);
    fclose(fp);
    close(sock_fd);
}
