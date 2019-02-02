#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES encryption in CBC-mode of operation.
// CTR enables encryption in counter-mode.
// ECB enables the basic ECB 16-byte block algorithm. All can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef CBC
  #define CBC 1
#endif

#ifndef ECB
  #define ECB 1
#endif

#ifndef CTR
  #define CTR 1
#endif


#define AES128 1


#define AES_BLOCKLEN 16 //Block length in bytes AES is 128b block only

  #define AES_KEYLEN 16   // Key length in bytes
  #define AES_keyExpSize 176

struct AES_ctx
{
  uint8_t RoundKey[AES_keyExpSize];
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
  uint8_t Iv[AES_BLOCKLEN];
#endif
};

typedef uint8_t state_t[4][4];

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);

#if defined(ECB) && (ECB == 1)
// buffer size is exactly AES_BLOCKLEN bytes; 
// you need only AES_init_ctx as IV is not used in ECB 
// NB: ECB is considered insecure for most uses
void AES_ECB_encrypt(struct AES_ctx* ctx, uint8_t* buf);
void AES_ECB_decrypt(struct AES_ctx* ctx, uint8_t* buf);

//Exposing functions for faulty injection
void AES_ECB_encrypt_fault(struct AES_ctx* ctx, uint8_t* buf, uint8_t col, uint8_t row, uint8_t fault);
void MixColumns(state_t* state);
void InvSubBytes(state_t* state);



uint8_t getSboxInvVal(uint8_t val);

#endif // #if defined(ECB) && (ECB == !)



#endif //_AES_H_
