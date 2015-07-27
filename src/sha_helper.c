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
