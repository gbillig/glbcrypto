/*
 * aes_family.c
 *
 *  Created on: Apr 10, 2016
 *      Author: glb
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../inc/aes_cipher.h"
#include "../inc/misc.h"

uint8_t aes(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode) {
	int i;
	int expanded_key_size;
	uint8_t *p;
	uint8_t expanded_key[240];

	expanded_key_size = (key_size + 28) * 4;
	p = aes_expand_key(key_size, key);

	for (i=0; i<expanded_key_size; i++) {
		expanded_key[i] = *(p+i);
	}

	if (mode == 0) {
		p = aes_encrypt(input_msg, expanded_key, key_size);
	} else {
		p = aes_decrypt(input_msg, expanded_key, key_size);
	}
	for (i=0; i<16; i++) {
		output_msg[i] = *(p+i);
	}

	return 0;
}

uint8_t aes_128(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode) {
	int rval;

	if (mode != 0 && mode != 1) {
		printf("aes_128 failed: invalid mode!\n");
		return EXIT_FAILURE;
	}

	if (key_size != 16) {
		printf("aes_128 failed: wrong key size!\n");
		return EXIT_FAILURE;
	}

	rval = aes(output_msg, input_msg, key, key_size, mode);
	return rval;
}

uint8_t aes_192(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode) {
	int rval;

	if (mode != 0 && mode != 1) {
		printf("aes_192 failed: invalid mode!\n");
		return EXIT_FAILURE;
	}

	if (key_size != 16) {
		printf("aes_192 failed: wrong key size!\n");
		return EXIT_FAILURE;
	}

	rval = aes(output_msg, input_msg, key, key_size, mode);
	return rval;
}

uint8_t aes_256(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode) {
	int rval;

	if (mode != 0 && mode != 1) {
		printf("aes_256 failed: invalid mode!\n");
		return EXIT_FAILURE;
	}

	if (key_size != 16) {
		printf("aes_256 failed: wrong key size!\n");
		return EXIT_FAILURE;
	}

	rval = aes(output_msg, input_msg, key, key_size, mode);
	return rval;
}
