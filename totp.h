#ifndef __TOTP__


#define HMAC_SHA1_HASH_SIZE 20
#define T0 0
#define VALIDITY 30

#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<ctype.h>
#include "utils.h"
#include "/home/usr/ssl/openssl/include/openssl/hmac.h"
#include "/home/usr/ssl/openssl/include/openssl/evp.h"



uint32_t totp(uint8_t *k, size_t keylen);



















#endif