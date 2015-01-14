/* this file should not be released as part of the API,
 * it is purely for internal use */

#ifndef IBCRYPT_BN_BIGNUM_UTIL_H
#define IBCRYPT_BN_BIGNUM_UTIL_H

#include <bignum.h>

#include <stdint.h>

/* some useful constant */
static uint64_t d1 = 1, d2 = 2;

static const BIGNUM ONE  = {&d1, 1};
static const BIGNUM TWO  = {&d2, 1};

int bnu_resize(BIGNUM* r, uint32_t size);

int bnu_trim(BIGNUM* r);

void bno_sub_no_resize(BIGNUM* r, const BIGNUM* a, const BIGNUM* b);
void bno_add_no_resize(BIGNUM* r, const BIGNUM* a, const BIGNUM* b);
int bno_rmod_no_resize(BIGNUM* r, const BIGNUM* n);

/* returns 1 if there was a carry, 0 if not */
int add_words(uint64_t* r, uint64_t* a, const uint32_t alen, uint64_t* b, const uint32_t blen);
/* returns 1 if there was a carry, 0 if not */
int sub_words(uint64_t* r, uint64_t* a, const uint32_t alen, uint64_t* b, const uint32_t blen);

void lshift_words(uint64_t* r, const uint64_t* a, uint32_t a_size, const uint64_t shift);
void rshift_words(uint64_t* r, const uint64_t* a, uint32_t a_size, const uint64_t shift);

int cmp_words(const uint64_t* a, const uint32_t alen, const uint64_t* b, const uint32_t blen);
int rmod_words(uint64_t* r, const uint32_t rlen, const BIGNUM* n);

/* barrett mod reduce operations */
int bno_barrett_reduce(BIGNUM* _r, const BIGNUM* a, const BIGNUM* m, const BIGNUM* n);
int bnu_barrett_mfactor(BIGNUM* r, const BIGNUM* n);

#endif
