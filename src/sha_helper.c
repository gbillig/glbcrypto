/*
 ============================================================================
 Name        : sha_helper.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

#include "../inc/aes.h"
#include "../inc/sha.h"

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {

	uint32_t result = (x & y) + (~x & z);
	return result;
}

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {

	uint32_t result = (x & y) + (x & z) + (y & z);
	return result;
}

uint32_t bit_rotation(uint32_t x, int shift) {
	int i;
	uint32_t result = x;

	for (i=0; i<shift; i++) {
		int bit = result & 1;
		result = result >> 1;
		result = result | (bit << 31);
	}

	return result;
}

uint32_t bit_shift(uint32_t x, int shift) {
	int i;
	uint32_t result = x;

	for (i=0; i<shift; i++) {
		result = result >> 1;
		result = result & 0x7FFFFFFF;
	}

	return result;
}

uint32_t big_sigma0(uint32_t x) {
	uint32_t result = bit_rotation(x, 2) + bit_rotation(x, 13) + bit_rotation(x, 22);
	return result;
}

uint32_t big_sigma1(uint32_t x) {
	uint32_t result = bit_rotation(x, 6) + bit_rotation(x, 11) + bit_rotation(x, 25);
	return result;
}

uint32_t small_sigma0(uint32_t x) {
	uint32_t result = bit_rotation(x, 7) + bit_rotation(x, 18) + bit_shift(x, 3);
	return result;
}

uint32_t small_sigma1(uint32_t x) {
	uint32_t result = bit_rotation(x, 17) + bit_rotation(x, 19) + bit_shift(x, 10);
	return result;
}
