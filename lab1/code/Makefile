CC = gcc
OPENSSL = ../openssl
INCLUDE = $(OPENSSL)/include 
CFLAGS = -c -I $(INCLUDE) 

all: p1

p1: aes_rsa_performance
	./aes_rsa_performance

.PHONY: all p1

aes_rsa_performance: aes_rsa_performance.c 
	$(CC) aes_rsa_performance.c -I$(INCLUDE) -o aes_rsa_performance $(OPENSSL)/libcrypto.a -ldl -lpthread

clean:
	rm -rf aes_rsa_performance