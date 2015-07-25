/*
 ============================================================================
 Name        : SHA.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../inc/SHA.h"

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
	rval = SHA256((int*) message, length);
}

int SHA256(char* message, uint64_t length) {

	int i,j;

	// length + 1 + k = 448 mod 512
	// k = 448 - (length + 1) mod 512
	int k = 448 - length - 1;
	while (k < 0) {
		k += 512;
	}

	//512 bit blocks
	int num_blocks = ceil((double) length / (double) 512);
	int* padded_msg = (int*) malloc((sizeof(int) * 64) * num_blocks);
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

	for (i=0; i<8; i++) {
		printf(BYTETOBINARYPATTERN" ", BYTETOBINARY(length_base256[i]));
	}
	printf("\n");


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

	return 0;
}
