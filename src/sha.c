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
#include <string.h>

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

const uint32_t K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

void SHA_test() {

	int rval;

	//issue: how to deal with messages that aren't multiple of 8 bits??
	//solution: ascii characters are 8-bits, so a string message will always have a length that is a multiple of 8 bits
	//edge case: hashing a binary number whose length isn't a multiple of 8 bits (ignore for now)

	char message[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	//char message[] = "abc";
	uint64_t length = strlen(message) * 8; //3 * 8 bits

	printf("SHA256 Test\nInput:\n%s\n", message);
	uint32_t* hash = SHA256((uint8_t*) message, length);

	printf("Output:\n");
	print_value_32(hash, 8);
	free(hash);
}

uint32_t* SHA256(uint8_t* message, uint64_t length) {

	int i,j;

	// length + 1 + k = 448 mod 512
	// k = 448 - (length + 1) mod 512
	int k = 448 - length - 1;
	while (k < 0) {
		k += 512;
	}

	int num_blocks = ceil((double) (length + 1 + k) / (double) 512);
	uint32_t** block = (uint32_t**) malloc(sizeof(uint32_t*) * num_blocks);
	uint32_t* padded_msg;

	padded_msg = SHA256_padding(message, length, k);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

	//byte swap is necessary if architecture is little endian
	//since we are converting an array of 8 bit integers to an array of 32 bit integers
	for (i=0; i<num_blocks*16; i++) {
		padded_msg[i] = ((padded_msg[i]>>24) & 0xff) | // move byte 3 to byte 0
						((padded_msg[i]<<8)  & 0xff0000) | // move byte 1 to byte 2
						((padded_msg[i]>>8)  & 0xff00) | // move byte 2 to byte 1
						((padded_msg[i]<<24) & 0xff000000); // byte 0 to byte 3
	}

#endif

	for (i=0; i<num_blocks; i++) {
		//padded message consists of 512 bit sized blocks, split into groups of 32 bits.
		//therefore, the start of each block is a multiple of 16 (16 * 32 = 512)
		block[i] = padded_msg + i*16;
	}

	//result : M(i)_j = block[i][j]

	uint32_t H1 = 0x6a09e667;
	uint32_t H2 = 0xbb67ae85;
	uint32_t H3 = 0x3c6ef372;
	uint32_t H4 = 0xa54ff53a;
	uint32_t H5 = 0x510e527f;
	uint32_t H6 = 0x9b05688c;
	uint32_t H7 = 0x1f83d9ab;
	uint32_t H8 = 0x5be0cd19;

	uint32_t a, b, c, d, e, f, g, h;

	for (i=0; i<num_blocks; i++) {
		a = H1;
		b = H2;
		c = H3;
		d = H4;
		e = H5;
		f = H6;
		g = H7;
		h = H8;

		uint32_t* W = expanded_message(block[i]);
		for (j=0; j<64; j++) {
			uint32_t T1 = h + big_sigma1(e) + Ch(e, f, g) + K[j] + W[j];
			uint32_t T2 = big_sigma0(a) + Maj(a, b, c);

			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		H1 = a + H1;
		H2 = b + H2;
		H3 = c + H3;
		H4 = d + H4;
		H5 = e + H5;
		H6 = f + H6;
		H7 = g + H7;
		H8 = h + H8;

		free(W);
	}

	uint32_t* hash = (uint32_t*) malloc(sizeof(uint32_t) * 8);
	hash[0] = H1;
	hash[1] = H2;
	hash[2] = H3;
	hash[3] = H4;
	hash[4] = H5;
	hash[5] = H6;
	hash[6] = H7;
	hash[7] = H8;

	free(padded_msg);
	free(block);
	return hash;
}

uint32_t* SHA256_padding(uint8_t* message, uint64_t length, int k) {
	int i,j;

	//512 bit blocks
	int num_blocks = ceil((double) (length + 1 + k) / (double) 512);
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

	/*
	for (i=0; i<num_blocks * 64; i++) {
		printf(BYTETOBINARYPATTERN" ", BYTETOBINARY(padded_msg[i]));
	}
	printf("\n");
	*/

	return (uint32_t*) padded_msg;
}


