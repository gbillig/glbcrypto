/*
 * mode_of_operation.c
 *
 *  Created on: Apr 10, 2016
 *      Author: glb
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
int cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode,
		int (*cipher)(uint8_t*, uint8_t*, uint8_t*, int, int)) {

	int i,j;

	uint8_t* in_state = malloc(sizeof(uint8_t) * 16);
	uint8_t* out_state = malloc(sizeof(uint8_t) * 16);
	uint8_t* iv_state = malloc(sizeof(uint8_t) * 16);

	for (i = 0; i < 16; i++) {
		iv_state[i] = iv[i];
	}

	for (i = 0; i*16 < msg_size; i++) {

		for (j = 0; j < 16; j++) {
			in_state[j] = input_msg[i * 16 + j];
			if (i != 0) {
				if (mode == 0) {
					iv_state[j] = output_msg[(i-1) * 16 + j];
				} else {
					iv_state[j] = input_msg[(i-1) * 16 + j];
				}
			}

			if (mode == 0) {
				in_state[j] = in_state[j] ^ iv_state[j];
			}
		}

		(*cipher)(out_state, in_state, key, key_size, mode);

		for (j = 0; j < 16; j++) {
			if (mode == 1) {
				output_msg[i * 16 + j] = out_state[j] ^ iv_state[j];
			} else {
				output_msg[i * 16 + j] = out_state[j];
			}
		}
	}

	free(in_state);
	free(out_state);
	free(iv_state);

	return 0;
}

int ecb(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* key, int key_size, int mode,
		int (*cipher)(uint8_t*, uint8_t*, uint8_t*, int, int)) {

	int i, j;

	uint8_t* in_state = malloc(sizeof(uint8_t) * 16);
	uint8_t* out_state = malloc(sizeof(uint8_t) * 16);

	for (i = 0; i * 16 < msg_size; i++) {
		for (j = 0; j < 16; j++) {
			in_state[j] = input_msg[i * 16 + j];
		}

		(*cipher)(out_state, in_state, key, key_size, mode);

		for (j = 0; j < 16; j++) {
			output_msg[i * 16 + j] = out_state[j];
		}
	}

	return 0;
}
