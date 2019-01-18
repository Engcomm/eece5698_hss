CC = gcc
OPENSSL = ../openssl
INCLUDE = $(OPENSSL)/include 
CFLAGS = -c -I $(INCLUDE) 



p1: aes_rsa_performance
	./aes_rsa_performance

.PHONY: p1

p3: aes_rsa_comms
	./aes_rsa_comms

aes_rsa_performance: aes_rsa_performance.c 
	$(CC) aes_rsa_performance.c -I$(INCLUDE) -o aes_rsa_performance $(OPENSSL)/libcrypto.a -ldl -lpthread

aes_rsa_comms: aes_rsa_comms.c
	$(CC) aes_rsa_comms.c -I$(INCLUDE) -o aes_rsa_comms $(OPENSSL)/libcrypto.a -ldl -lpthread


clean:
	rm -rf aes_rsa_performance