#ifndef IBCRYPT_RAND_H
#define IBCRYPT_RAND_H

#define URANDOM_FAIL 1

#include <stdlib.h>
#include <stdint.h>

// returns URANDOM_FAIL if unsuccessful
int cs_rand(void* buf, size_t buflen);

uint32_t cs_rand_int();

uint32_t cs_rand_int_range(uint32_t top);

#endif