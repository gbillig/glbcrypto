/*
 ============================================================================
 Name        : sha.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../inc/sha.h"
#include "../inc/misc.h"

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

void SHA_test() {

	int rval;

	//issue: how to deal with messages that aren't multiple of 8 bits??
	//solution: ascii characters are 8-bits, so a string message will always have a length that is a multiple of 8 bits
	//edge case: hashing a binary number whose length isn't a multiple of 8 bits (ignore for now)
	char message[] = "abc";
	uint64_t length = 24; //3 * 8 bits
	rval = SHA256((uint8_t*) message, length);
}

int SHA256(uint8_t* message, uint64_t length) {

	int i;
	int num_blocks = ceil((double) length / (double) 512);
	uint32_t** block = (uint32_t**) malloc(sizeof(uint32_t*) * num_blocks);
	uint32_t* padded_msg;

	padded_msg = SHA256_padding(message, length);

	for (i=0; i<num_blocks; i++) {
		//padded message consists of 512 bit sized blocks, split into groups of 32 bits.
		//therefore, the start of each block is a multiple of 16 (16 * 32 = 512)
		block[i] = padded_msg + i*16;
	}

	//result : M(i)_j = block[i][j]

	uint32_t H1[4] = {0x6a, 0x09, 0xe6, 0x67};
	uint32_t H2[4] = {0xbb, 0x67, 0xae, 0x85};
	uint32_t H3[4] = {0x3c, 0x6e, 0xf3, 0x72};
	uint32_t H4[4] = {0xa5, 0x4f, 0xf5, 0x3a};
	uint32_t H5[4] = {0x51, 0x0e, 0x52, 0x7f};
	uint32_t H6[4] = {0x9b, 0x05, 0x68, 0x8c};
	uint32_t H7[4] = {0x1f, 0x83, 0xd9, 0xab};
	uint32_t H8[4] = {0x5b, 0xe0, 0xcd, 0x19};

	uint32_t a[4], b[4], c[4], d[4], e[4], f[4], g[4], h[4];

	for (i=1; i<=num_blocks; i++) {
		copy_array_32(H1, a, 4);
		copy_array_32(H2, b, 4);
		copy_array_32(H3, c, 4);
		copy_array_32(H4, d, 4);
		copy_array_32(H5, e, 4);
		copy_array_32(H6, f, 4);
		copy_array_32(H7, g, 4);
		copy_array_32(H8, h, 4);
	}

	return 0;
}

uint32_t* SHA256_padding(uint8_t* message, uint64_t length) {
	int i,j;

	// length + 1 + k = 448 mod 512
	// k = 448 - (length + 1) mod 512
	int k = 448 - length - 1;
	while (k < 0) {
		k += 512;
	}

	//512 bit blocks
	int num_blocks = ceil((double) length / (double) 512);
	uint8_t* padded_msg = (uint8_t*) malloc((sizeof(uint8_t) * 64) * num_blocks);
	int num_full_blocks = (num_blocks-1);
	int last_block_length = length - num_full_blocks * 512;

	//conversion from base 10 to base 256 (2^8)
	int num_digits = 1;
	uint64_t quotient = length / 256;
	while (quotient > 0) {
		quotient = quotient / 256;
		num_digits++;
	}

	int length_base256[8] = {0};
	quotient = length;
	for (i=8-num_digits; i<8; i++) {
		//index = start_index (8-num_digits) + end_index (7) - current index (i)
		//this reverses the list we iterate over
		length_base256[8-num_digits + 7 - i] = quotient % 256;
		quotient = quotient / 256;
	}


	//section where padded message is the same as the original message
	int interval_1 = num_full_blocks * 512 / 8 + last_block_length / 8;
	//section where 0b10000000 is appended
	int interval_2 = interval_1 + 1;
	//section where zeroes are appended
	int interval_3 = interval_2 + ((k - 7) / 8);
	//section where length in binary is appended
	int interval_4 = interval_3 + 8;


	int counter = 0;
	for (i=0; i<num_blocks * 64; i++) {
		if (i < interval_1) {
			padded_msg[i] = message[i];
		} else if (i < interval_2) {
			//0x80 = 0b10000000
			padded_msg[i] = 0x80;
		} else if (i < interval_3) {
			padded_msg[i] = 0;
		} else if (i < interval_4) {
			padded_msg[i] = length_base256[counter];
			counter++;
		}
	}

	for (i=0; i<num_blocks * 64; i++) {
		printf(BYTETOBINARYPATTERN" ", BYTETOBINARY(padded_msg[i]));
	}
	printf("\n");

	return (uint32_t*) padded_msg;
}


