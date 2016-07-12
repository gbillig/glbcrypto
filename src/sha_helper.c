/*
 ============================================================================
 Name        : sha_helper.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../inc/aes_cipher.h"
#include "../inc/sha.h"

uint32_t Ch_32(uint32_t x, uint32_t y, uint32_t z) {

	uint32_t result = (x & y) ^ (~x & z);
	return result;
}

uint64_t Ch_64(uint64_t x, uint64_t y, uint64_t z) {

	uint64_t result = (x & y) ^ (~x & z);
	return result;
}

uint32_t Maj_32(uint32_t x, uint32_t y, uint32_t z) {

	uint32_t result = (x & y) ^ (x & z) ^ (y & z);
	return result;
}

uint64_t Maj_64(uint64_t x, uint64_t y, uint64_t z) {

	uint64_t result = (x & y) ^ (x & z) ^ (y & z);
	return result;
}

uint32_t bit_rotation_32(uint32_t x, int shift) {
	uint32_t result = x;

	int mask = (1 << shift) - 1;
	int lsb = result & mask;
	result = result >> shift;
	result = result | (lsb << (32 - shift));

	return result;
}

uint64_t bit_rotation_64(uint64_t x, int shift) {
	uint64_t result = x;

	uint64_t mask = 1;
	mask = (mask << shift) - 1;
	uint64_t lsb = result & mask;
	result = result >> shift;
	result = result | (lsb << (64 - shift));

	return result;
}

uint32_t bit_shift_32(uint32_t x, int shift) {
	if (shift == 0) {
		return x;
	}

	uint32_t result;

	int mask = (1 << (32 - shift)) - 1;
	result = x >> shift;
	result = result & mask;

	return result;
}

uint64_t bit_shift_64(uint64_t x, int shift) {
	if (shift == 0) {
		return x;
	}

	uint64_t result;

	uint64_t mask = 1;
	mask = (mask << (64 - shift)) - 1;
	result = x >> shift;
	result = result & mask;

	return result;
}

uint32_t big_sigma0_32(uint32_t x) {
	uint32_t result = bit_rotation_32(x, 2) ^ bit_rotation_32(x, 13) ^ bit_rotation_32(x, 22);
	return result;
}

uint64_t big_sigma0_64(uint64_t x) {
	uint64_t result = bit_rotation_64(x, 28) ^ bit_rotation_64(x, 34) ^ bit_rotation_64(x, 39);
	return result;
}

uint32_t big_sigma1_32(uint32_t x) {
	uint32_t result = bit_rotation_32(x, 6) ^ bit_rotation_32(x, 11) ^ bit_rotation_32(x, 25);
	return result;
}

uint64_t big_sigma1_64(uint64_t x) {
	uint64_t result = bit_rotation_64(x, 14) ^ bit_rotation_64(x, 18) ^ bit_rotation_64(x, 41);
	return result;
}

uint32_t small_sigma0_32(uint32_t x) {
	uint32_t result = bit_rotation_32(x, 7) ^ bit_rotation_32(x, 18) ^ bit_shift_32(x, 3);
	return result;
}

uint64_t small_sigma0_64(uint64_t x) {
	uint64_t result = bit_rotation_64(x, 1) ^ bit_rotation_64(x, 8) ^ bit_shift_64(x, 7);
	return result;
}

uint32_t small_sigma1_32(uint32_t x) {
	uint32_t result = bit_rotation_32(x, 17) ^ bit_rotation_32(x, 19) ^ bit_shift_32(x, 10);
	return result;
}

uint64_t small_sigma1_64(uint64_t x) {
	uint64_t result = bit_rotation_64(x, 19) ^ bit_rotation_64(x, 61) ^ bit_shift_64(x, 6);
	return result;
}

uint32_t* expanded_message_32(uint32_t* M) {
	int i;
	uint32_t* W = (uint32_t*) malloc(sizeof(uint32_t) * 64);

	for (i=0; i<64; i++) {
		if (i < 16) {
			W[i] = M[i];
		} else {
			W[i] = small_sigma1_32(W[i-2]) + W[i-7] + small_sigma0_32(W[i-15]) + W[i-16];
		}
	}

	return W;
}

uint64_t* expanded_message_64(uint64_t* M) {
	int i;
	uint64_t* W = (uint64_t*) malloc(sizeof(uint64_t) * 80);

	for (i=0; i<80; i++) {
		if (i < 16) {
			W[i] = M[i];
		} else {
			W[i] = small_sigma1_64(W[i-2]) + W[i-7] + small_sigma0_64(W[i-15]) + W[i-16];
		}
	}

	return W;
}
