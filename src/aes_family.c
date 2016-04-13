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
#include "../inc/mode_of_operation.h"

int aes(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode) {
	uint8_t* expanded_key = malloc(sizeof(uint8_t) * 240);

	aes_expand_key(expanded_key, key, key_size);

	if (mode == 0) {
		aes_encrypt(output_msg, input_msg, expanded_key, key_size);
	} else {
		aes_decrypt(output_msg, input_msg, expanded_key, key_size);
	}

	free(expanded_key);

	return 0;
}

int aes_128(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode) {
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

int aes_192(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode) {
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

int aes_256(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode) {
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

int aes_128_cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode) {
	int i,j;
	if (msg_size % 16 != 0) {
		printf("aes_128_cbc failed: invalid message size!\n");
		return EXIT_FAILURE;
	}

	if (iv == NULL) {
		get_random(iv, 16);
	}

	uint8_t* in_state = malloc(sizeof(uint8_t) * 16);
	uint8_t* out_state = malloc(sizeof(uint8_t) * 16);

	int (*aes_128_pointer)(uint8_t*, uint8_t*, uint8_t*, int, int) = &aes_128;

	for (i = 0; i*16 < msg_size; i++) {

		for (j = 0; j < 16; j++) {
			in_state[j] = input_msg[i * 16 + j];
			if (i != 0) {
				iv[j] = output_msg[(i-1) * 16 + j];
			}
		}

		cbc(out_state, in_state, iv, key, key_size, mode, aes_128_pointer);

		for (j = 0; j < 16; j++) {
			output_msg[i * 16 + j] = out_state[j];
		}
	}

	free(in_state);
	free(out_state);

	return 0;
}
