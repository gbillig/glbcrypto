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
#include "../inc/sha_helper.h"
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

const uint32_t K_sha256[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

const uint64_t K_sha512[80] = {
		0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
		0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
		0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
		0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
		0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
		0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
		0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
		0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
		0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
		0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
		0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
		0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
		0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
		0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
		0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
		0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
		0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
		0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
		0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
		0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817,
};

void sha_testcase() {

	int rval, i;

	//issue: how to deal with messages that aren't multiple of 8 bits??
	//solution: ascii characters are 8-bits, so a string message will always have a length that is a multiple of 8 bits
	//edge case: hashing a binary number whose length isn't a multiple of 8 bits (ignore for now)

	//char message[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	char message[] = "abc";
	uint64_t length = strlen(message) * 8; //3 * 8 bits

	printf("SHA256 Test 1\nInput:\n%s\n", message);
	uint8_t* hash32 = (uint8_t*) malloc(sizeof(uint8_t) * 32);
	sha_256(hash32, (uint8_t*) message, length);

	printf("Output:\n");
	print_value_8(hash32, 32);
	free(hash32);

	// 1421 = 0b10110001101 (11 bits)
	// QUESTION: is the data encoded {10110001, 00000101} or {10110001, 10100000} ?
	// Currently, the implementation uses the first option.
	uint8_t message2[] = {177, 5};
	length = 11; //11 bits

	printf("SHA256 Test 2\nInput:\n%d\n", message2[0]);
	sha_256(hash32, (uint8_t*) message2, length);

	printf("Output:\n");
	print_value_8(hash32, 32);
	free(hash32);

	char message3[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	length = strlen(message3) * 8; // each char is 8 bits

	printf("SHA512 Test\nInput:\n%s\n", message3);
	uint8_t* hash64 = (uint8_t*) malloc(sizeof(uint8_t) * 64);
	sha_512(hash64, (uint8_t*) message3, length);

	printf("Output:\n");
	print_value_8(hash64, 64);
	free(hash64);
}

int sha_256(uint8_t* output_msg, uint8_t* input_msg, uint64_t length) {

	int i,j;

	// length + 1 + k = 448 mod 512
	// k = 448 - (length + 1) mod 512
	int k = 448 - length - 1;
	while (k < 0) {
		k += 512;
	}

	//512-bit blocks
	int padded_msg_length = length + 1 + k + 64;
	int num_blocks =  padded_msg_length / 512;

	uint32_t** block = (uint32_t**) malloc(sizeof(uint32_t*) * num_blocks);
	uint8_t* padded_msg = (uint8_t*) malloc(sizeof(uint8_t) * padded_msg_length / 8);

	sha_256_pad_message(padded_msg, input_msg, length, k);
	uint32_t* padded_msg_32 = (uint32_t*) padded_msg;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

	//byte swap is necessary if architecture is little endian
	//since we are converting an array of 8 bit integers to an array of 32 bit integers
	for (i=0; i<num_blocks*16; i++) {
		padded_msg_32[i] = ((padded_msg_32[i]>>24) & 0xff)      | // move byte 3 to byte 0
						((padded_msg_32[i]>>8)  & 0xff00)    | // move byte 2 to byte 1
						((padded_msg_32[i]<<8)  & 0xff0000)  | // move byte 1 to byte 2
						((padded_msg_32[i]<<24) & 0xff000000); // move byte 0 to byte 3
	}

#endif

	for (i=0; i<num_blocks; i++) {
		//padded message consists of 512 bit sized blocks, split into groups of 32 bits.
		//the pointers to the beginning of each block is at intervals of 16 (16 * 32-bits = 512 bit blocks)
		block[i] = padded_msg_32 + i*16;
	}

	//result : M(i)_j = block[i][j]

	uint32_t a, b, c, d, e, f, g, h;
	uint32_t H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

	for (i=0; i<num_blocks; i++) {
		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];

		uint32_t* W = expanded_message_32(block[i]);
		for (j=0; j<64; j++) {
			uint32_t T1 = h + big_sigma1_32(e) + Ch_32(e, f, g) + K_sha256[j] + W[j];
			uint32_t T2 = big_sigma0_32(a) + Maj_32(a, b, c);

			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		H[0] = a + H[0];
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
		H[5] = f + H[5];
		H[6] = g + H[6];
		H[7] = h + H[7];

		free(W);
	}

	//convert 32-bit variables into 8-bit output array
	for (i = 0; i < 8; i++) {
		output_msg[i * 4 + 0] = H[i] >> 24;
		output_msg[i * 4 + 1] = H[i] >> 16;
		output_msg[i * 4 + 2] = H[i] >> 8;
		output_msg[i * 4 + 3] = H[i];
	}

	free(padded_msg);
	free(block);
	return 0;
}

int sha_512(uint8_t* output_msg, uint8_t* input_msg, uint64_t length) {

	int i,j;

	// length + 1 + k = 896 mod 1024
	// k = 896 - (length + 1) mod 1024
	int k = 896 - length - 1;
	while (k < 0) {
		k += 1024;
	}

	//1024-bit blocks
	int padded_msg_length = length + 1 + k + 128;
	int num_blocks =  padded_msg_length / 1024;
	uint64_t** block = (uint64_t**) malloc(sizeof(uint64_t*) * num_blocks);
	uint8_t* padded_msg = (uint8_t*) malloc(sizeof(uint8_t) * padded_msg_length / 8);

	sha_512_pad_message(padded_msg, input_msg, length, k);

	uint64_t* padded_msg_64 = (uint64_t*) padded_msg;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

	//byte swap is necessary if architecture is little endian
	//since we are converting an array of 8 bit integers to an array of 64 bit integers
	for (i=0; i<num_blocks*16; i++) {
		padded_msg_64[i] = ((padded_msg_64[i]>>56) & 0xff)              | // move byte 7 to byte 0
						((padded_msg_64[i]>>40) & 0xff00)            | // move byte 6 to byte 1
						((padded_msg_64[i]>>24) & 0xff0000)          | // move byte 5 to byte 2
						((padded_msg_64[i]>>8)  & 0xff000000)        | // move byte 4 to byte 3
						((padded_msg_64[i]<<8)  & 0xff00000000)      | // move byte 3 to byte 4
						((padded_msg_64[i]<<24) & 0xff0000000000)    | // move byte 1 to byte 5
						((padded_msg_64[i]<<40) & 0xff000000000000)  | // move byte 2 to byte 6
						((padded_msg_64[i]<<56) & 0xff00000000000000); // move byte 0 to byte 7
	}

#endif

	for (i=0; i<num_blocks; i++) {
		//padded message consists of 1024 bit sized blocks, split into groups of 64 bits.
		//the pointers to the beginning of each block is at intervals of 32 (16 * 64-bits = 1024 bit blocks)
		block[i] = padded_msg_64 + i*16;
	}

	//result : M(i)_j = block[i][j]

	uint64_t a, b, c, d, e, f, g, h;
	uint64_t H[8] = {0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
					 0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179};

	for (i=0; i<num_blocks; i++) {
		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];

		uint64_t* W = expanded_message_64(block[i]);
		for (j=0; j<80; j++) {
			uint64_t T1 = h + big_sigma1_64(e) + Ch_64(e, f, g) + K_sha512[j] + W[j];
			uint64_t T2 = big_sigma0_64(a) + Maj_64(a, b, c);

			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		H[0] = a + H[0];
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
		H[5] = f + H[5];
		H[6] = g + H[6];
		H[7] = h + H[7];

		free(W);
	}

	//convert 64-bit variables into 8-bit output array
	for (i = 0; i < 8; i++) {
		output_msg[i * 8 + 0] = H[i] >> 56;
		output_msg[i * 8 + 1] = H[i] >> 48;
		output_msg[i * 8 + 2] = H[i] >> 40;
		output_msg[i * 8 + 3] = H[i] >> 32;
		output_msg[i * 8 + 4] = H[i] >> 24;
		output_msg[i * 8 + 5] = H[i] >> 16;
		output_msg[i * 8 + 6] = H[i] >> 8;
		output_msg[i * 8 + 7] = H[i];
	}

	free(padded_msg);
	free(block);
	return 0;
}

int sha_256_pad_message(uint8_t* padded_msg, uint8_t* msg, uint64_t msg_length, int k) {
	int i,j;

	//message gets split into groups of 8 bits fit into the padded_msg array
	for (i = 0; i < msg_length / 8; i++) {
		padded_msg[i] = msg[i];
	}

	int num_padding_zeroes = k;
	int msg_remainder_by_eight = msg_length % 8;

	//if input isn't a multiple of 8 bits (rare edge case)
	if (msg_remainder_by_eight != 0) {
		//message mask is the remainder number of leading 1s followed by zeroes
		//example for remainder 3: 0b11100000
		uint8_t message_mask = ((1 << msg_remainder_by_eight) - 1) << (8 - msg_remainder_by_eight);

		//shift message to align the value with the MSB (left side)
		uint8_t msg_section = (msg[i] << (8-msg_remainder_by_eight)) & message_mask;

		//append the "1" and complete the 8-bit value with zeroes
		padded_msg[i] = msg_section | (1 << (7-msg_remainder_by_eight));
		num_padding_zeroes -= 7 - msg_remainder_by_eight;
	} else {
		//0x80 = 0b10000000
		padded_msg[i] = 0x80;
		num_padding_zeroes -= 7;
	}

	for (i = i + 1; num_padding_zeroes > 0; num_padding_zeroes -= 8, i++ ) {
		padded_msg[i] = 0;
	}

	//converting msg_length from base 10 to base 256 (2^8) to fit 8-bit groups
	int num_digits = 1;
	uint64_t quotient = msg_length / 256;
	while (quotient > 0) {
		quotient = quotient / 256;
		num_digits++;
	}

	int msg_length_base256[8] = {0};
	quotient = msg_length;
	for (j = 8 - num_digits; j < 8; j++) {
		//index = start_index (8-num_digits) + end_index (7) - current index (i)
		//this reverses the list we iterate over
		msg_length_base256[8 - num_digits + 7 - j] = quotient % 256;
		quotient = quotient / 256;
	}

	for (j = 0; j < 8; i++, j++) {
		padded_msg[i] = msg_length_base256[j];
	}

	/*
	for (i=0; i < padded_msg_length / 8; i++) {
		printf(BYTETOBINARYPATTERN" ", BYTETOBINARY(padded_msg[i]));
	}
	printf("\n");
	*/

	return 0;
}

//msg_length can be up to 128 bits long, but this implementation only supports 64-bit variables
int sha_512_pad_message(uint8_t* padded_msg, uint8_t* msg, uint64_t msg_length, int k) {
	int i,j;

	//message gets split into groups of 8 bits fit into the padded_msg array
	for (i = 0; i < msg_length / 8; i++) {
		padded_msg[i] = msg[i];
	}

	int num_padding_zeroes = k;
	int msg_remainder_by_eight = msg_length % 8;

	//if input isn't a multiple of 8 bits
	if (msg_remainder_by_eight != 0) {
		//message mask is the remainder number of leading 1s followed by zeroes
		//example for remainder 3: 0b11100000
		uint8_t message_mask = ((1 << msg_remainder_by_eight) - 1) << (8 - msg_remainder_by_eight);

		//shift message to align the value with the MSB (left side)
		uint8_t msg_section = (msg[i] << (8-msg_remainder_by_eight)) & message_mask;

		//append the "1" and complete the 8-bit value with zeroes
		padded_msg[i] = msg_section | (1 << (7-msg_remainder_by_eight));
		num_padding_zeroes -= 7 - msg_remainder_by_eight;
	} else {
		//0x80 = 0b10000000
		padded_msg[i] = 0x80;
		num_padding_zeroes -= 7;
	}

	for (i = i + 1; num_padding_zeroes > 0; num_padding_zeroes -= 8, i++ ) {
		padded_msg[i] = 0;
	}

	//msg_length is only supported for lengths up to 64-bits
	//thus we fill the 64 most significant bits with zeroes
	// 64 bits == 8 bytes
	for (j = 0; j < 8; j++, i++) {
		padded_msg[i] = 0;
	}

	//converting msg_length from base 10 to base 256 (2^8) to fit 8-bit groups
	int num_digits = 1;
	uint64_t quotient = msg_length / 256;
	while (quotient > 0) {
		quotient = quotient / 256;
		num_digits++;
	}

	int msg_length_base256[8] = {0};
	quotient = msg_length;
	for (j = 8 - num_digits; j < 8; j++) {
		//index = start_index (8-num_digits) + end_index (7) - current index (i)
		//this reverses the list we iterate over
		msg_length_base256[8 - num_digits + 7 - j] = quotient % 256;
		quotient = quotient / 256;
	}

	for (j = 0; j < 8; i++, j++) {
		padded_msg[i] = msg_length_base256[j];
	}

	/*
	for (i=0; i< padded_msg_length / 8; i++) {
		printf(BYTETOBINARYPATTERN" ", BYTETOBINARY(padded_msg[i]));
	}
	printf("\n");
	*/

	return 0;
}
