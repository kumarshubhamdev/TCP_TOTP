#include "totp.h"

#define HMAC_SHA1_HASH_SIZE 20
#define T0 0
#define VALIDITY 30


uint8_t
*hmac(unsigned char *key, int kl, uint64_t interval)
{

    return (uint8_t *)HMAC(EVP_sha1(), key, kl,
            (const unsigned char *)&interval, sizeof(interval), NULL, 0);
}


uint32_t
DT(uint8_t *digest)
{

    uint64_t offset;
    uint32_t bin_code;


    // dynamically truncates hash
    offset   = digest[19] & 0x0f;

    bin_code = (digest[offset]  & 0x7f) << 24
        | (digest[offset+1] & 0xff) << 16
        | (digest[offset+2] & 0xff) <<  8
        | (digest[offset+3] & 0xff);

    return bin_code;

}



uint32_t HOTP(uint8_t *key, size_t kl, uint64_t interval, int digits)
{

    uint8_t *digest;
    uint32_t result;
    uint32_t endianness;


    endianness = 0xdeadbeef;
    if ((*(const uint8_t *)&endianness) == 0xef) {
        interval = ((interval & 0x00000000ffffffff) << 32) | ((interval & 0xffffffff00000000) >> 32);
        interval = ((interval & 0x0000ffff0000ffff) << 16) | ((interval & 0xffff0000ffff0000) >> 16);
        interval = ((interval & 0x00ff00ff00ff00ff) <<  8) | ((interval & 0xff00ff00ff00ff00) >>  8);
    };


   
    digest = (uint8_t *)hmac(key, kl, interval);
    uint32_t dbc = DT(digest);


    return dbc;

}


uint32_t totp(uint8_t *k, size_t keylen)
{
    time_t t = floor((time(NULL) - T0) / VALIDITY);
    uint32_t result;
    
    result=HOTP(k,keylen,t,6);
    
    int power=pow(10,6);
    uint32_t otp = result%power;
    
    return otp;

    
    
}



