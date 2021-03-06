#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../misc/zfree.h"
#include "bignum.h"
#include "bignum_util.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

/* size is in 64 bit blocks */
int bnu_resize(bignum *r, uint32_t size) {
	if(r->size == size) {
		return 0;
	}

	uint64_t *ptr;
	/* check for malloc fail */
	if(size != 0) {
		ptr = malloc(sizeof(uint64_t) * (uint64_t) size);
		if(ptr == NULL) {
			return 1;
		}
	} else {
		ptr = 0;
	}

	if(r->d != 0) {
		memcpy(ptr, r->d, sizeof(uint64_t) * (uint64_t) min(r->size, size));
		zfree(r->d, sizeof(uint64_t) * (uint64_t) r->size);
	}

	r->d = ptr;

	/* zero allocated space */
	for(uint32_t i = r->size; i < size; i++) {
		r->d[i] = 0;
	}
	r->size = size;
	return 0;
}

int bnu_trim(bignum *r) {
	if(r == NULL) {
		return -1;
	}

	if(r->size == 0) {
		return 0;
	}

	int64_t i = r->size; /* use signed so that we can go lower than 0 */
	do {
		i--;
	} while(i >= 0 && r->d[i] == 0);

	return bnu_resize(r, (uint32_t) (i + 1));
}

int bnu_free(bignum *r) {
	if(r == NULL) {
		return -1;
	}

	return bnu_resize(r, 0);
}

static inline char fhex(const uint8_t v) {
	if(v < 10) {
		return (char) (v + '0');
	} else {
		return (char) (v + 'a' - 10);
	}
}

int bnu_tstr(char *out, const bignum *a) {
	uint32_t i = a->size;
	size_t pos = 0;

	while(i--) {
		for(int j = 15; j >= 0; j--) {
			uint8_t val = (a->d[i] & (0xfULL << (j * 4))) >> (j * 4);
			out[pos] = fhex(val);
			pos++;
		}
	}

	out[pos] = '\0';

	return 0;
}

int bnu_print(const bignum *a) {
	char* str = malloc(16 * a->size + 1);
	bnu_tstr(str, a);
	printf("%s", str);
	free(str);
	return 0;
}

