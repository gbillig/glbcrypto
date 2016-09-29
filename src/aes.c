/*
 * aes.c
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

	if (key_size != 24) {
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

	if (key_size != 32) {
		printf("aes_256 failed: wrong key size!\n");
		return EXIT_FAILURE;
	}

	rval = aes(output_msg, input_msg, key, key_size, mode);
	return rval;
}

int aes_128_cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode) {
	int rval;

	if (msg_size % 16 != 0) {
		printf("aes_128_cbc failed: invalid message size!\n");
		return EXIT_FAILURE;
	}

	if (iv == NULL) {
		get_random(iv, 16);
	}

	int (*aes_128_pointer)(uint8_t*, uint8_t*, uint8_t*, int, int) = &aes_128;

	rval = cbc(output_msg, input_msg, msg_size, iv, key, key_size, mode, aes_128_pointer);

	return rval;
}

int aes_192_cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode) {
	int rval;

	if (msg_size % 16 != 0) {
		printf("aes_192_cbc failed: invalid message size!\n");
		return EXIT_FAILURE;
	}

	if (iv == NULL) {
		get_random(iv, 16);
	}

	int (*aes_192_pointer)(uint8_t*, uint8_t*, uint8_t*, int, int) = &aes_192;

	rval = cbc(output_msg, input_msg, msg_size, iv, key, key_size, mode, aes_192_pointer);

	return rval;
}

int aes_256_cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode) {
	int rval;

	if (msg_size % 16 != 0) {
		printf("aes_256_cbc failed: invalid message size!\n");
		return EXIT_FAILURE;
	}

	if (iv == NULL) {
		get_random(iv, 16);
	}

	int (*aes_256_pointer)(uint8_t*, uint8_t*, uint8_t*, int, int) = &aes_256;

	rval = cbc(output_msg, input_msg, msg_size, iv, key, key_size, mode, aes_256_pointer);

	return rval;
}

int aes_128_ecb(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* key, int key_size, int mode) {
	int rval;

	if (msg_size % 16 != 0) {
		printf("aes_128_ecb failed: invalid message size!\n");
		return EXIT_FAILURE;
	}

	int (*aes_128_pointer)(uint8_t*, uint8_t*, uint8_t*, int, int) = &aes_128;

	rval = ecb(output_msg, input_msg, msg_size, key, key_size, mode, aes_128_pointer);

	return rval;
}

int aes_192_ecb(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* key, int key_size, int mode) {
	int rval;

	if (msg_size % 16 != 0) {
		printf("aes_192_ecb failed: invalid message size!\n");
		return EXIT_FAILURE;
	}

	int (*aes_192_pointer)(uint8_t*, uint8_t*, uint8_t*, int, int) = &aes_192;

	rval = ecb(output_msg, input_msg, msg_size, key, key_size, mode, aes_192_pointer);

	return rval;
}

int aes_256_ecb(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* key, int key_size, int mode) {
	int rval;

	if (msg_size % 16 != 0) {
		printf("aes_256_ecb failed: invalid message size!\n");
		return EXIT_FAILURE;
	}

	int (*aes_256_pointer)(uint8_t*, uint8_t*, uint8_t*, int, int) = &aes_256;

	rval = ecb(output_msg, input_msg, msg_size, key, key_size, mode, aes_256_pointer);

	return rval;
}
