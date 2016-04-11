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

int cbc(uint8_t* output, uint8_t* input, uint8_t* iv, uint8_t* key, int key_size, int mode,
		int (*cipher)(uint8_t*, uint8_t*, uint8_t*, int, int)) {

	int i;
	uint8_t* in_state = malloc(sizeof(uint8_t) * 16);
	uint8_t* out_state = malloc(sizeof(uint8_t) * 16);


	for (i = 0; i < 16; i++) {
		if (mode) {
			in_state[i] = input[i] ^ iv[i];
		} else {
			in_state[i] = input[i];
		}
	}

	(*cipher)(out_state, in_state, key, key_size, mode);

	for (i = 0; i < 16; i++) {
		if (mode) {
			output[i] = out_state[i];
		} else {
			output[i] = out_state[i] ^ iv[i];
		}
	}

	free(in_state);
	free(out_state);

	return 0;
}
