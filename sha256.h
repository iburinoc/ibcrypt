#ifndef IBUR_SHA256_H
#define IBUR_SHA256_H

#define SHA_256_DEBUG 0

void hash_sha256(const unsigned char* const message, const unsigned long size, unsigned char* const out);

#endif
