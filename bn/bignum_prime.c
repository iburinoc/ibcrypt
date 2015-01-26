#include <bignum.h>
#include "bignum_util.h"
#include <rand.h>

static const size_t num_small_primes = 167;
static const uint64_t *const small_primes;
static int divisible_single_digit(const bignum *n, const uint64_t q);

int rabin_miller(int *r, const bignum *n, const uint32_t certainty);

int prime_test(int *r, const bignum *n, const uint32_t certainty) {
	if(n == NULL || r == NULL) {
		return 1;
	}

	/* check small numbers */
	if(n->size == 0) {
		*r = 0;
		return 0;
	}
	if(n->size == 1) {
		if(n->d[0] == 1) {
			*r = 0;
			return 0;
		} else if(n->d[0] == 2) {
			*r = 1;
			return 0;
		}
		size_t i;
		for(i = 0; i < num_small_primes; i++) {
			if(n->d[0] == small_primes[i]) {
				*r = 1;
				return 0;
			}
		}
	}

	/* check divisibility by 2 */
	if(!(n->d[0] & 1)) {
		*r = 0;
		return 0;
	}

	/* check small primes */
	size_t i;
	for(i = 0; i < num_small_primes; i++) {
		if(divisible_single_digit(n, small_primes[i])) {
			*r = 0;
			return 0;
		}
	}

	/* fall back to rabin miller */
	return rabin_miller(r, n, certainty);
}

/* runs rabin-miller primality test on the number
 * http://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
 */
int rabin_miller(int *r, const bignum *n, const uint32_t certainty) {
	if(n == NULL) {
		return 1;
	}

	/* get 1 and n - 1 in bignum form for comparison */
	bignum n_minus_one = BN_ZERO;
	if(bno_sub(&n_minus_one, n, &ONE) != 0) {
		return 1;
	}

	/* find n=2^s * d where d is odd */
	/* find the first set bit of a */
	uint64_t i, j;
	for(i = 0; i < n_minus_one.size; i++) {
		for(j = 0; j < 64; j++) {
			if(n_minus_one.d[i] & (1ULL << j)) {
				goto dloopend;
			}
		}
	}
	dloopend:;
	const uint64_t s = i * 64 + j;
	bignum d = BN_ZERO;

	if(bno_rshift(&d, &n_minus_one, s) != 0) {
		return 1;
	}

	bignum a = BN_ZERO;
	bignum x = BN_ZERO;

	/* each iteration has a 1/4 chance of false positive */
	/* run ceil(certainty/2) times to obtain the certainty value*/
	const uint64_t iters = (certainty + 1) / 2;
	for(i = 0; i < iters; i++) {
		if(bni_rand_range(&a, &TWO, &n_minus_one) != 0) {
			return 1;
		}
		if(bno_exp_mod(&x, &a, &d, n) != 0) {
			return 1;
		}

		if(bno_cmp(&x, &ONE) == 0 || bno_cmp(&x, &n_minus_one) == 0) {
			goto testloopend;
		}

		for(j = 0; j < s - 1; j++) {
			if(bno_mul_mod(&x, &x, &x, n) != 0) {
				return 1;
			}
			if(bno_cmp(&x, &ONE) == 0) {
				*r = 0;
				goto end;
			}
			if(bno_cmp(&x, &n_minus_one) == 0) {
				goto testloopend;
			}
		}
		*r = 0;
		goto end;

		testloopend:;
		printf("test %llu\n", i);
	}
	*r = 1;

end:

	bnu_free(&n_minus_one);
	bnu_free(&d);
	bnu_free(&a);
	bnu_free(&x);

	return 0;
}

/* sets r to 1 if probably prime, 0 if definitely composite
 * the odds that this method returns a false positive is at most 2^(-certainty) */
int bnu_probably_prime(int *r, const bignum *a, const uint32_t certainty) {
	return 1;
}

int bnu_gen_prime(bignum *r, const int bitlen) {
	return bnu_trim(r);
}

/* test if a bignum is divisible by a given single digit number using long
 * division
 * note that this method won't work with digits larger than half the word size
 */
static int divisible_single_digit(const bignum *n, const uint64_t q) {
	if(n->size == 0) return 1;

	uint32_t i = n->size - 1;
	uint64_t r = n->d[i] % q;

	for(i--; i < (uint32_t) 0xffffffffU; i--) {
		r = ((r << 32) | (n->d[i] >> 32)) % q;
		r = ((r << 32) | (n->d[i] & (uint64_t) 0xffffffffU)) % q;
	}

	return r == 0;
}

static const uint64_t small_primes_array[] = {
3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79,
83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163,
167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251,
257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443,
449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647,
653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757,
761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983,
991, 997
};

static const uint64_t *const small_primes = (const uint64_t *const) small_primes_array;

